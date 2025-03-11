/*
 * RenderingEngine.hpp
 * 
 * Include file for the Rendering Engine
 */

namespace Render {
	// function declarations

	/// @brief Main engine function, handles creating and handling the render loop
	/// @param framerate The application framerate, measured in frames per second
	/// @return Returns true if an error occurs, false otherwise
	bool render(int, int[]);
}