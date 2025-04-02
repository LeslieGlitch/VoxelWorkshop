/*
 * PhysicsEngine.cpp
 *
 * Runs a loop at a configurable tickrate that handles creating and updating a physics scene
 */

#include <iostream>
#include "PhysicsEngine.hpp"

namespace Physics {
    bool physics(int tickrate) {
        // Initialization
        std::cout << "Physics function called";
        bool quitScene = false;
        ///@TODO iterate over objects and call start()

        // Main Physics Loop
        while (!quitScene) {
            ///@TODO iterate over objects and call update()
        }

        // Cleanup
        return 0;
    }
}

