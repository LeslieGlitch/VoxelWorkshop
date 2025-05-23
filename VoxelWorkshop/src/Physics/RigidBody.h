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

    /// @brief Copy Constructor
    /// @param source Object to be copied
    RigidBody(const RigidBody& source);

    RigidBody operator=(const RigidBody& right);

    /// @brief Apply a force impulse to the object
    /// @param impulse Total impulse in N*s
    /// @param direction The direction of the force applied
    /// @param offsetFromCenterOfMass Where the force is applied relative to the center of mass
    void Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass = glm::vec3(0.0, 0.0, 0.0));

    /// @brief Function called when object spawns
    void start();

    /// @brief Function called once every game tick
    void update();

    void detectCollision(const Object& collider);

    static std::string getType();

    /// @brief Apply gravitational force in a certain direction
    /// @param direction Direction gravity acts
    void applyDirectionGravity(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f));

    /// @brief Apply gravitational force towards a certain point
    /// @param pointLocation Point of attraction
    void applyPointGravity(glm::vec3 pointLocation = glm::vec3(0.0f, 0.0f, 0.0f));
};

#endif
