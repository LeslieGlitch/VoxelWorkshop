/*
 * main.cpp
 * 
 * Starting point for VoxelWorkshop engine
 */

#include <iostream>
#include <thread>

#include "Voxels/Chunk.h"
#include "RenderingEngine.hpp"

// SETTINGS
int VIEWPORT_SIZE[] = { 800, 600 };
const int MAX_FRAMERATE = 60; // in fps

int main() {
	// Start other threads
	std::thread RenderThread(Render::render, MAX_FRAMERATE, VIEWPORT_SIZE);

	// wait for RenderThread to terminate before continuing
	RenderThread.join();

	return 0;
}
