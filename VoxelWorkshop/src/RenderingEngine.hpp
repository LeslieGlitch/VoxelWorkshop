/*
 * RenderingEngine.hpp
 * 
 * Include file for the Rendering Engine
 */

#include <GLFW/glfw3.h>

namespace Render {
	// function declarations

	/// @brief Main engine function, handles creating and handling the render loop
	/// @param framerate The application framerate, measured in frames per second
	/// @return Returns true if an error occurs, false otherwise
	bool render(int, int[]);


	bool init(int[]);


	void framebuffer_size_callback(GLFWwindow* window, int width, int height);


	void processInput(GLFWwindow* window);
}