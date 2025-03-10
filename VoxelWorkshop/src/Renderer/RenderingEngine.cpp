/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

// Turn on GLM experiments
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderingEngine.hpp"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

#include "../Math/Math.h"
#include "../Voxels/Chunk.h"

const unsigned int screenSize[] = { 800, 800 };

 // Vertices coordinates
GLfloat vertices[] =
{//    Coordinates		/     Colors           /   L/R - U/D - F/B
	-0.5f,  0.0f, -0.5f,   0.00f, 0.00f, 0.00f, // L   - D   - B
	 0.5f,  0.0f, -0.5f,   0.99f, 0.00f, 0.00f, // R   - D   - B
	-0.5f,  1.0f, -0.5f,   0.00f, 0.99f, 0.00f, // L   - U   - B
	 0.5f,  1.0f, -0.5f,   0.99f, 0.99f, 0.00f, // R   - U   - B
	-0.5f,  0.0f,  0.5f,   0.00f, 0.00f, 0.99f, // L   - D   - F
	 0.5f,  0.0f,  0.5f,   0.99f, 0.00f, 0.99f, // R   - D   - F
	-0.5f,  1.0f,  0.5f,   0.00f, 0.99f, 0.99f, // L   - U   - F
	 0.5f,  1.0f,  0.5f,   0.99f, 0.99f, 0.99f  // R   - U   - F
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 2,
	0, 1, 3,
	0, 2, 6,
	0, 6, 4,
	0, 4, 1,
	1, 4, 5,
	4, 6, 5,
	5, 6, 7,
	1, 5, 3,
	3, 5, 7,
	2, 3, 7,
	2, 7, 6
};

namespace Render {
	// local variables
	const GLdouble pi = 3.1415926535897932384626433832795;

	bool render(int framerate, int windowSize[]) {
		/* Initialization */

		// Initialize GLFW
		glfwInit();

		// Tell GLFW what version of OpenGL we are using 
		// In this case we are using OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		// Tell GLFW we are using the CORE profile
		// So that means we only have the modern functions
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create a GLFWwindow object, naming it "Voxel Workshop"
		GLFWwindow* window = glfwCreateWindow(screenSize[0], screenSize[1], "Voxel Workshop", NULL, NULL);
		// Error check if the window fails to create
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		// Introduce the window into the current context
		glfwMakeContextCurrent(window);

		//Load GLAD so it configures OpenGL
		gladLoadGL();
		// Specify the viewport of OpenGL in the Window
		// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
		glViewport(0, 0, screenSize[0], screenSize[1]);

		/* Shaders */

		// Generates Shader object using shaders defualt.vert and default.frag
		Shader shaderProgram("src/Shaders/default.vert", "src/Shaders/default.frag");

		// Generates Vertex Array Object and binds it
		VAO VAO1;
		VAO1.Bind();

		// Generates Vertex Buffer Object and links it to vertices
		VBO VBO1(vertices, sizeof(vertices));
		// Generates Element Buffer Object and links it to indices
		EBO EBO1(indices, sizeof(indices));

		// Links VBO attributes (like coords and color) to VAO
		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		// Unbind all to prevent accidentally modifying them
		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();

		// Enable depth buffer
		glEnable(GL_DEPTH_TEST);

		Camera camera(screenSize[0], screenSize[1], glm::vec3(0.0f, 0.0f, 2.0f));

		/* Render Loop */
		while (!glfwWindowShouldClose(window))
		{
			// Process user input
			processInput(window);
			// Specify the color of the background
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Tell OpenGL which Shader Program we want to use
			shaderProgram.Activate();
			// Collect inputs from user
			camera.Inputs(window);
			// Transform coordinates based on camera position
			camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

			// Bind the VAO so OpenGL knows to use it
			VAO1.Bind();
			// Draw primitives, number of indices, datatype of indices, index of indices
			glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
			// Take care of all GLFW events
			glfwPollEvents();
		}

		/* Cleanup */

		// Delete all the objects we've created
		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();
		shaderProgram.Delete();
		// Delete window before ending the program
		glfwDestroyWindow(window);
		// Terminate GLFW before ending the program
		glfwTerminate();
		return 0;
	}

	/// @brief Starting setup for rendering engine
	/// @return Returns true if an error occurs, false otherwise
	bool init(int windowSize[]) {

		return false;
	}

	/* Callback Functions */

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure width and height are nonzero
		if (width  == 0) width  = 1;
		if (height == 0) height = 1;

		// update viewport size
		glViewport(0, 0, width, height);
		return;
	}

	/* Helper Functions */

	void processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}
