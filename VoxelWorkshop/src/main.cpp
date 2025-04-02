/*
 * main.cpp
 * 
 * Starting point for VoxelWorkshop engine
 */

#include <iostream>
#include <thread>

#include "Renderer/RenderingEngine.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Voxels/Brickgrid.h"

// SETTINGS
int VIEWPORT_SIZE[] = { 800, 600 };
const int MAX_FRAMERATE = 60; // in fps
const int MAX_TICKRATE = 30; // in tps

int main() {
    // Start other threads
    std::thread RenderThread(Render::render, MAX_FRAMERATE, VIEWPORT_SIZE);
    std::thread PhysicsThread(Physics::physics, MAX_TICKRATE);

    // wait for threads to terminate before continuing
    RenderThread.join();
    PhysicsThread.join();

    return 0;
}
