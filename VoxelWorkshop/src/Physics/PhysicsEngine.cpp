/*
 * PhysicsEngine.cpp
 *
 * Runs a loop at a configurable tickrate that handles creating and updating a physics scene
 */

#include <iostream>
#include "PhysicsEngine.hpp"

bool quitScene = false;

namespace Physics {
    bool physics(int tickrate) {
        // Initialization
        std::cout << "Physics function called";
        ///@TODO iterate over objects and call start()

        // Main Physics Loop
        while (!quitScene) {
            ///@TODO iterate over objects and call update()
        }

        // Cleanup
        return 0;
    }

    void quitPhysics() {
        quitScene = true;
    }
}

