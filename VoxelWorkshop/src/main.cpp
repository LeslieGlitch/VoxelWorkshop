/*
 * main.cpp
 * 
 * Starting point for VoxelWorkshop engine
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Voxels/Chunk.h"

// SETTINGS
const int VIEWPORT_SIZE[] = { 800, 600 };

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	// initialize GLFW
	if (!glfwInit()) {
		return -1;
	}

	// openGL ver. 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// use core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	// separate settings for apple devices
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#endif

	// create window
	GLFWwindow* window = glfwCreateWindow(VIEWPORT_SIZE[0], VIEWPORT_SIZE[1], "VoxelWorkshop", NULL, NULL);

	// guard clause: failure to instantiate window
	if (window == NULL) {
		std::cout << "ERROR: Could not create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// focus on new window
	glfwMakeContextCurrent(window);

	// vsync
	glfwSwapInterval(0);

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR: Failure to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// pass rendering window size to OpenGL
	glViewport(0, 0, VIEWPORT_SIZE[0], VIEWPORT_SIZE[1]);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// send new frame to window
		glfwSwapBuffers(window);

		// poll for events and process
		glfwPollEvents();
	}

	// clear GLFW resources on close
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	return;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
