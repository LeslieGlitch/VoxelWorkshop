/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

#include <iostream>

const int MAX_FRAMERATE = 60; // in fps

bool init();

/// @brief Main engine function, handles creating and handling the render loop
/// @param framerate The application framerate, measured in frames per second
/// @return Returns true if an error occurs, false otherwise
bool render(int framerate) {
	// Initialize
	if (init()) {
		// Failure to initialize
		std::cerr << "ERROR: Render engine failed to initialize!\n"
			<< "Returning to";
		return true;
	}

	// Main render loop
	bool done = false;
	while (!done) {
		done = true;
	}

	return false;
}

bool init() {
	return true;
}
