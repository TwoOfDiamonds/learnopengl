// learnopengl.cpp : Defines the entry point for the console application.
//

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <chrono>
#include <cmath>
#include <memory>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Camera.h"
#include "ObjFileParser.h"
#include "SceneNode.h"

constexpr double PI = 3.14159265;
constexpr double DEG_TO_RAD = PI / 180.0;

GLuint mousePositionUniformLocation = 0;
int windowwidth = 800, windowheight = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowwidth = width;
	windowheight = height;
	glViewport(0, 0, windowwidth, windowheight);
}

std::unique_ptr<Camera> camera;
unsigned int currentBatman = 0;
//glm::mat4 matrix;
float distance = -1.f;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
	t_start = t_now;

	camera->HandleInput(window, deltaTime);

	//Current batman controls
	/*if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		currentBatman = (currentBatman + 1) % batmans.size();
	}
*/
	//Camera controls
	//if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
	//{
	//	camera->Translate(glm::vec3(0.f, -1.f * time, 0.f));
	//}
	//if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
	//{
	//	camera->Translate(glm::vec3(0.f, 1.f * time, 0.f));
	//}
	//if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
	//{
	//	camera->Translate(glm::vec3(-1.f * time, 0.f, 0.f));
	//}
	//if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
	//{
	//	camera->Translate(glm::vec3(1.f * time, 0.f, 0.f));
	//}
	//if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
	//{
	//	camera->Translate(glm::vec3(0.f, 0.f, -1.f * time));
	//}
	//if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
	//{
	//	camera->Translate(glm::vec3(0.f, 0.f, 1.f * time));
	//}
}

std::string ReadFile(std::string filename)
{
	std::ifstream file(filename);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(windowwidth, windowheight, "Hello World", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//creating vertex shader
	std::string vertexString = ReadFile("basic.vert");
	const char* vertexShaderSource = vertexString.c_str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//creating fragment shader
	std::string fragmentString = ReadFile("basic.frag");
	const char* fragmentShaderSource = fragmentString.c_str();
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	assert(glGetError() == GL_NO_ERROR);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	assert(glGetError() == GL_NO_ERROR);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//creating and linking shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	assert(glGetError() == GL_NO_ERROR);
	glLinkProgram(shaderProgram);
	assert(glGetError() == GL_NO_ERROR);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	assert(glGetError() == GL_NO_ERROR);
	glUseProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	int error = glGetError();
	assert(error == GL_NO_ERROR);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	GLuint uniTrans = glGetUniformLocation(shaderProgram, "transMat");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	uint32_t LightPosUniformLocation = glGetUniformLocation(shaderProgram, "lightPos");
	float lightPosition[3] = { 0.2f, 0.5f, 2.f };
	glUniform3fv(LightPosUniformLocation, 1, lightPosition);

	ObjFileParser objFile("batman/bunny.obj");
	std::shared_ptr<SceneNode> objectRoot = objFile.GetRoot();

	std::vector<SceneNode::SceneNodePtr> bunnies;
	SceneNode sceneRoot;

	for (int i = -5; i < 5; i++)
	{
		for (int j = -5; j < 5; j++)
		{
			glm::vec3 position((float)i, 0.f, (float)j);
			std::shared_ptr<SceneNode> bunny(std::make_shared<SceneNode>(*objectRoot));
			bunny->Translate(position);
			bunnies.push_back(bunny);
			sceneRoot.AttachChild(bunny);
		}
	}

	glm::mat4 projection = glm::perspective(120.f, (float)windowwidth / (float)windowheight, 0.01f, 100.f);
	camera = std::make_unique<Camera>(projection);
	camera->Translate(glm::vec3(0.f, 0.f, -1.f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		camera->Draw(shaderProgram, sceneRoot);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

