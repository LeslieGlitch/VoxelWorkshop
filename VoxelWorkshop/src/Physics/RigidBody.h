/*
 * RigidBody.h
 *
 * Include file for Rigid Body class
 */

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Object.h"

class RigidBody : public Object {
public:
    /// @brief Default Constructor
    RigidBody();

    /// @brief Constructor with brickmap filename
    RigidBody(std::string filename);

    /// @brief Contructor with Brickmap
    RigidBody(Brickmap structure);

    /// @brief Function called when object spawns
    void start();

    /// @brief Function called once every game tick
    void update();
};

#endif
