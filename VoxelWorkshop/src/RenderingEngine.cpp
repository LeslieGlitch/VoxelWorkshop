/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.hpp"
#include "Math/Math.h"
#include "Voxels/Chunk.h"

namespace Render {
	// local variables
	GLFWwindow* window = NULL;
	const GLdouble pi = 3.1415926535897932384626433832795;

	// function declarations
	static void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	static Math::Matrix4 lookAt(Math::Vector3& eye, Math::Vector3& target, Math::Vector3& upDir);

	bool render(int framerate, int windowSize[]) {
		// Initialize
		if (init(windowSize)) {
			// Failure to initialize
			std::cerr << "ERROR: Render engine failed to initialize!\n"
				<< "Returning to";
			return true;
		}

		// Create test chunk
		int chunkLoc[] = { 0, 0, 0 };
		Chunk* testChunk = new Chunk(chunkLoc);

		while (!glfwWindowShouldClose(window)) {
			// process input
			processInput(window);

			// clear screen
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// set up projection matrix
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			perspectiveGL(45.0, (double)windowSize[0] / (double)windowSize[1], 1.0, 100.0);

			// set up modelview matrix
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			Math::Vector3 eyePos = Math::Vector3(0.0, 0.0, 0.0);
			Math::Vector3 lookPos = Math::Vector3(0.0, 0.0, 0.0);
			Math::Vector3 upVec = Math::Vector3(0.0, 0.0, 0.0);
			lookAt(eyePos,  // eye position
				   lookPos,  // position to look at
				   upVec); // up direction

			// send render command to chunk
			testChunk->Render();

			// send new frame to window
			glfwSwapBuffers(window);

			// poll for events and process
			glfwPollEvents();
		}

		// clear GLFW resources on close
		glfwDestroyWindow(window);
		glfwTerminate();

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
		if (!window) {
			std::cout << "ERROR: Could not create window." << std::endl;
			glfwTerminate();
			return true;
		}

		// focus on new window
		glfwMakeContextCurrent(window);

		// vsync
		glfwSwapInterval(1);

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
		// make sure width and height are nonzero
		if (width  == 0) width  = 1;
		if (height == 0) height = 1;

		// update viewport size
		glViewport(0, 0, width, height);
		return;
	}

	void processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	/// @brief Replaces gluPerspective. Sets the frustum to perspective mode.
	/// @param fovY Field of vision in degrees in the y direction
	/// @param aspect Aspect ratio of the viewport
	/// @param zNear The near clipping distance
	/// @param zFar The far clipping distance
	static void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
		GLdouble fW, fH;
		fH = tan(fovY / 360 * pi) * zNear;
		fW = fH * aspect;
		glFrustum(-fW, fW, -fH, fH, zNear, zFar);
	}

	static Math::Matrix4 lookAt(Math::Vector3& eye, Math::Vector3& target, Math::Vector3& upDir) {
		// compute the forward vector from target to eye
		Math::Vector3 forward = eye - target;
		forward.normalize();                 // make unit length

		// compute the left vector
		Math::Vector3 left = upDir.cross(forward); // cross product
		left.normalize();

		// recompute the orthonormal up vector
		Math::Vector3 up = forward.cross(left);    // cross product

		// init 4x4 matrix
		Math::Matrix4 matrix;
		matrix.identity();

		// set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform
		matrix.m[0] = left.x();
		matrix.m[4] = left.y();
		matrix.m[8] = left.z();
		matrix.m[1] = up.x();
		matrix.m[5] = up.y();
		matrix.m[9] = up.z();
		matrix.m[2] = forward.x();
		matrix.m[6] = forward.y();
		matrix.m[10] = forward.z();

		// set translation part
		matrix.m[12] = -left.x() * eye.x() - left.y() * eye.y() - left.z() * eye.z();
		matrix.m[13] = -up.x() * eye.x() - up.y() * eye.y() - up.z() * eye.z();
		matrix.m[14] = -forward.x() * eye.x() - forward.y() * eye.y() - forward.z() * eye.z();

		return matrix;
	}
}
