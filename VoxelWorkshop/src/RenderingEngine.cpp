/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.hpp"

namespace Render {
	GLFWwindow* window = NULL;

	bool render(int framerate, int windowSize[]) {
		// Initialize
		if (init(windowSize)) {
			// Failure to initialize
			std::cerr << "ERROR: Render engine failed to initialize!\n"
				<< "Returning to";
			return true;
		}

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

		// Main render loop
		bool done = false;
		while (!done) {
			done = true;
		}

		return false;
	}

	/// @brief Starting setup for rendering engine
	/// @return Returns true if an error occurs, false otherwise
	bool init(int windowSize[]) {
		// initialize GLFW
		if (!glfwInit()) {
			return true;
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
		window = glfwCreateWindow(windowSize[0], windowSize[1], "VoxelWorkshop", NULL, NULL);

		// guard clause: failure to instantiate window
		if (window == NULL) {
			std::cout << "ERROR: Could not create window." << std::endl;
			glfwTerminate();
			return true;
		}

		// focus on new window
		glfwMakeContextCurrent(window);

		// vsync
		glfwSwapInterval(0);

		// initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "ERROR: Failure to initialize GLAD" << std::endl;
			glfwTerminate();
			return true;
		}

		// pass rendering window size to OpenGL
		glViewport(0, 0, windowSize[0], windowSize[1]);

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		return false;
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
}
