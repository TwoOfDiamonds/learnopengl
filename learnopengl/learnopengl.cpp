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

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "TGAFile.h"

constexpr double PI = 3.14159265;
constexpr double DEG_TO_RAD = PI / 180.0;

GLuint mousePositionUniformLocation = 0;
int windowwidth = 800, windowheight = 800;

void stuff(glm::mat4 &blabla)
{
	blabla = glm::mat4(1.f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowwidth = width;
	windowheight = height;
	glViewport(0, 0, windowwidth, windowheight);
}

std::vector<float> GetCircleBuffer(float radius, float r, float g, float b, int nooftriangles)
{
	assert(nooftriangles > 2);

	std::vector<float> buffer;
	
	buffer.push_back(0.f); buffer.push_back(0.f); buffer.push_back(0.f);
	buffer.push_back(r); buffer.push_back(g); buffer.push_back(b);
	/*buffer.push_back(radius); buffer.push_back(0.f); buffer.push_back(0.f);
	buffer.push_back(r); buffer.push_back(g); buffer.push_back(b);
*/
	float gradesPerTriangle = 360.f / nooftriangles;

	for (int i = 0; i <= nooftriangles; i++)
	{
		float point = gradesPerTriangle * i;
		float pointRad = point * DEG_TO_RAD;

		//cos and sin folosesc radiani
		buffer.push_back(cos(pointRad) * radius);
		buffer.push_back(sin(pointRad) * radius);
		buffer.push_back(0.f);
		buffer.push_back(r); buffer.push_back(g); buffer.push_back(b);
	}

	return buffer;
}

std::vector<float> GetDoughnutBuffer(float radius, float holeRadius, float r, float g, float b, float nooftriangles)
{
	std::vector<float> buffer;

	float gradesPerTriangle = 360.f / nooftriangles;

	for (int i = 0; i <= nooftriangles; i++)
	{
		float point = gradesPerTriangle * i;
		float pointRad = point * DEG_TO_RAD;

		//cos and sin folosesc radiani
		buffer.push_back(cos(pointRad) * holeRadius);
		buffer.push_back(sin(pointRad) * holeRadius);
		buffer.push_back(0.f);
		buffer.push_back(r); buffer.push_back(g); buffer.push_back(b);

		buffer.push_back(cos(pointRad) * radius);
		buffer.push_back(sin(pointRad) * radius);
		buffer.push_back(0.f);
		buffer.push_back(r); buffer.push_back(g); buffer.push_back(b);
	}

	return buffer;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	std::vector<float> vertices = {
		0.0f, -0.5f, -0.5f,		1.0f,	0.0f,	0.0f,
		0.5f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f, 0.0f,		1.f,	0.0f,	0.0f,
		
		0.0f, 0.5f, -0.25f,		0.0f, 1.0f,	0.0f,
		0.0f, -0.5f, -0.5f,		0.0f, 1.0f,	0.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f,	0.0f,

		0.0f, 0.5f, -0.25f,		0.0f, 0.0f,	1.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,	1.0f,
		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,	1.0f,
	};

	//std::vector<float> vertices = GetCircleBuffer(0.5f, 1.0f, 0.0f, 0.0f, 72);
	//std::vector<float> vertices = GetDoughnutBuffer(0.5f, 0.2f, 1.0f, 0.0f, 0.0f, 72);

	//creating vertex attribute object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//creating vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	GLuint uniTrans = glGetUniformLocation(shaderProgram, "transMat");

	auto t_start = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		glm::mat4 trans = glm::mat4(1.f);
		trans = glm::rotate(trans, time * glm::radians(180.f), glm::vec3(0.f, 1.f, 0.5f));
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

		processInput(window);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		//glBindTexture(GL_TEXTURE_2D, texture);
		
		glBindVertexArray(VAO);		

		//glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

