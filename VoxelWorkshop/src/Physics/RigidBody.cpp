/*
 * RigidBody.cpp
 *
 * Implementation file for Rigid Body class
 */

#define GLM_ENABLE_EXPERIMENTAL

#include "RigidBody.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

RigidBody::RigidBody() {
    RigidBody::structure.loadFromFile("hSphere.bm");
    indexArraySize = RigidBody::structure.generateMesh(RigidBody::location, RigidBody::material);
    // Link mesh to the VAO
    RigidBody::structure.linkMesh();
}

RigidBody::RigidBody(std::string filename) {
    RigidBody::structure.loadFromFile(filename);
    indexArraySize = RigidBody::structure.generateMesh(RigidBody::location, RigidBody::material);
    // Link mesh to the VAO
    RigidBody::structure.linkMesh();
}

RigidBody::RigidBody(Brickmap structure) {
    RigidBody::structure = structure;
}

void RigidBody::start() {
    // Perform Base start logic
    Object::start();
    return;
}

void RigidBody::update() {
    // Perform Base update logic
    Object::update();

    // Update location data
    LocationData newLocation = RigidBody::location;
    PhysicsData newPhysics = RigidBody::movement;
    newLocation.Position += RigidBody::movement.linearVelocity;
    newPhysics.linearVelocity += RigidBody::movement.linearAcceleration;
    newPhysics.linearAcceleration = glm::normalize(-RigidBody::location.Position) * 0.01f;

    // Update rotation data
    newLocation.Rotation += RigidBody::movement.rotationalVelocity;
    newPhysics.rotationalVelocity += RigidBody::movement.rotationalAcceleration;
    newPhysics.rotationalAcceleration = glm::vec4(1.0, 1.0, 1.0, 0.0);

    setPhysics(newPhysics);
    setTransformation(newLocation);

    return;
}

void RigidBody::Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass) {
    // Create data to emplace
    //LocationData newLocation = RigidBody::location;
    PhysicsData newPhysics = RigidBody::movement;

    // Update linear acceleration
    newPhysics.linearAcceleration += glm::normalize(direction) * impulse;

    // Update rotation acceleration
    if (offsetFromCenterOfMass.length() > 0.1) {
        glm::vec3 rotAxis = glm::normalize(glm::cross(offsetFromCenterOfMass, direction));
        newPhysics.rotationalAcceleration += glm::vec4(rotAxis, impulse/250);
    }

    setPhysics(newPhysics);
    //setTransformation(newLocation);
}
