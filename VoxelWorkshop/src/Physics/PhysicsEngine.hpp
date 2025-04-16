/*
 * PhysicsEngine.hpp
 *
 * Include file for the Physics Engine
 */

#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP

#include "../Voxels/Brickgrid.h"
#include "../Physics/RigidBody.h"
#include "../Physics/StaticBody.h"

namespace Physics {
    // function declarations

    /// @brief Main physics engine function, handles creating and handling the physics loop
    /// @param tickrate The application tickrate, measured in ticks per second
    /// @return Returns true if an error occurs, false otherwise
    bool physics(int);

    void quitPhysics();
}

#endif
