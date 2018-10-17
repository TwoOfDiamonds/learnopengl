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

std::vector<SceneNode::SceneNodePtr> batmans;
unsigned int currentBatman = 0;
//glm::mat4 matrix;
float distance = -3.f;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
	t_start = t_now;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		batmans[currentBatman]->Rotate(glm::radians(180.f) * time, glm::vec3(0.f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		batmans[currentBatman]->Rotate(glm::radians(180.f) * time * -1.f, glm::vec3(0.f, 1.0f, 0.0f));
	}	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		batmans[currentBatman]->Rotate(glm::radians(180.f) * time, glm::vec3(1.f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		batmans[currentBatman]->Rotate(glm::radians(180.f) * time * -1.f, glm::vec3(1.f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		distance += time * 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		distance += time * -1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		batmans[currentBatman]->Translate(glm::vec3(0.f, -1.f * time, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		batmans[currentBatman]->Translate(glm::vec3(0.f, 1.f * time, 0.f));
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		batmans[currentBatman]->Translate(glm::vec3(-1.f * time, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		batmans[currentBatman]->Translate(glm::vec3(1.f * time, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		currentBatman = (currentBatman + 1) % batmans.size();
	}
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

	ObjFileParser objFile("batman/batman.obj");

	SceneNode sceneRoot;
	std::shared_ptr<SceneNode> batmanRoot = objFile.GetRoot();
	std::shared_ptr<SceneNode> secondBatmanRoot = std::make_shared<SceneNode>(*batmanRoot);
	sceneRoot.AttachChild(batmanRoot);
	sceneRoot.AttachChild(secondBatmanRoot);

	batmans.push_back(batmanRoot);
	batmans.push_back(secondBatmanRoot);

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

	glm::mat4 transFactor = glm::mat4(1.0f);
	transFactor = glm::translate(transFactor, glm::vec3(0.f, 0.f, distance));

	for (auto& batman : batmans)
	{
		batman->SetTransformationMatrix(transFactor);
	}

	glm::mat4 projection = glm::perspective(120.f, (float)windowwidth / (float)windowheight, 0.01f, 100.f);
	Camera camera(projection);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		camera.Draw(shaderProgram, sceneRoot);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

