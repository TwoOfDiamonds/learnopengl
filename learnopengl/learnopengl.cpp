// learnopengl.cpp : Defines the entry point for the console application.
//

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "TGAFile.h"

unsigned int mousePixelPosUniformLocation = 0;
unsigned int mousePosUniformLocation = 0;
unsigned int radiusUniformLocation = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	float pos[2] = { static_cast<float>(xpos), static_cast<float>(ypos) };
	glUniform2fv(mousePixelPosUniformLocation, 1, pos);

	xpos = 2.0 * xpos / 1240.0 - 1.0;
	ypos = (2.0 * ypos / 800.0 - 1.0) * -1.0;

	//double halfWidth = 1240.0 / 2.0;
	//double halfHeight = 800.0 / 2.0;
	//xpos = (xpos - halfWidth) / halfWidth;
	//ypos = (ypos - halfHeight) / halfHeight * -1.0;

	pos[0] = static_cast<float>(xpos);
	pos[1] = static_cast<float>(ypos);

	std::cout << pos[0] << ", " << pos[1] << std::endl;

	glUniform2fv(mousePosUniformLocation, 1, pos);
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
	TGAFile texFile("TGA32bpp.tga");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1240, 800, "Hello World", nullptr, nullptr);

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

	glViewport(0, 0, 1240, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		-100.f, -100.f, -100.f, -100.f, -100.f,

		-0.5f, -0.5f, 0.0f,		0.2f, -0.2f, 0.0f,	0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.0f,		-0.2f, 0.2f, 0.0f,	0.5f, 0.5f, 0.5f,
		0.0f, 0.5f, 0.0f,		0.0f, 0.2f, 0.0f,	0.5f, 0.5f, 0.5f,
		
		-0.75f, 0.75f, 0.0f,	0.0f, 0.0f, 0.0f,	0.5f, 0.5f, 0.5f,
		-0.25f, 0.75f, 0.0f,	0.0f, 0.0f, 0.0f,	0.5f, 0.5f, 0.5f,
		-0.50f, -0.75f, 0.0f,	0.0f, 0.0f, 0.0f,	0.5f, 0.5f, 0.5f,

		0.7f, -0.3f, 0.0f,		0.2f, -0.2f, 0.0f,	0.5f, 0.5f, 0.5f,
		0.9f, -0.8f, 0.0f,		-0.2f, 0.2f, 0.0f,	0.5f, 0.5f, 0.5f,
		0.6f, -0.8f, 0.0f,		0.0f, 0.2f, 0.0f,	0.5f, 0.5f, 0.5f,
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	mousePixelPosUniformLocation = glGetUniformLocation(shaderProgram, "mousePixelPosition");
	mousePosUniformLocation = glGetUniformLocation(shaderProgram, "mousePosition");
	radiusUniformLocation = glGetUniformLocation(shaderProgram, "radius");
	float radius = 300;
	glUniform1f(radiusUniformLocation, radius);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	int error = glGetError();
	assert(error == GL_NO_ERROR);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}


	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)20);
	std::cout << glGetError() << std::endl;
	assert(glGetError() == GL_NO_ERROR);
	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)32);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)44);
	glEnableVertexAttribArray(2);

	//glEnable(GL_TEXTURE_2D);

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format = texFile.GetPixelDepth() == 32 ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, texFile.GetWidth(), texFile.GetHeight(), 0, format, GL_UNSIGNED_BYTE, texFile.GetImageData());

	unsigned int textureUniformLocation = glGetUniformLocation(shaderProgram, "myTexture");
	glUniform1i(textureUniformLocation, 0);

	unsigned int otherTexture;
	glGenTextures(1, &otherTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, otherTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	TGAFile otherFile("arta.tga");
	format = otherFile.GetPixelDepth() == 32 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, otherFile.GetWidth(), otherFile.GetHeight(), 0, format, GL_UNSIGNED_BYTE, otherFile.GetImageData());

	unsigned int otherTextureUniformLocation = glGetUniformLocation(shaderProgram, "myOtherTexture");
	glUniform1i(otherTextureUniformLocation, 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glfwSetCursorPosCallback(window, mouse_pos_callback);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		//glBindTexture(GL_TEXTURE_2D, texture);
		
		glBindVertexArray(VAO);		

		glDrawArrays(GL_TRIANGLES, 0, 18);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

