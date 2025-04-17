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
    newLocation.Position += RigidBody::movement.linearVelocity * delta;
    newPhysics.linearVelocity += RigidBody::movement.linearAcceleration * delta;
    newPhysics.linearAcceleration = glm::normalize(-RigidBody::location.Position) * 9.8f;

    // Update rotation data
    newLocation.Rotation += RigidBody::movement.rotationalVelocity * delta;
    newPhysics.rotationalVelocity += RigidBody::movement.rotationalAcceleration * delta;
    newPhysics.rotationalAcceleration = glm::vec4(1.0, 1.0, 1.0, 0.0);

    setPhysics(newPhysics);
    setTransformation(newLocation);

    return;
}

#include <iostream>
void RigidBody::Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass) {
    // Create data to emplace
    //LocationData newLocation = RigidBody::location;
    PhysicsData newPhysics = RigidBody::movement;

    // Update linear acceleration
    std::cout << "linAcc before: " << glm::length(newPhysics.linearAcceleration) << "\n";
    newPhysics.linearAcceleration += glm::normalize(direction) * (impulse / (RigidBody::mass() * delta));
    std::cout << "Mass:          " << RigidBody::mass() << "\n";
    std::cout << "linAcc after:  " << glm::length(newPhysics.linearAcceleration) << "\n\n";

    // Update rotation acceleration
    if (glm::length(offsetFromCenterOfMass) > 0.1) {
        std::cout << "rotation is happening\n\n";
        glm::vec3 rotAxis = glm::normalize(glm::cross(offsetFromCenterOfMass, direction));
        float projCoef = glm::dot(glm::normalize(direction), glm::vec3(glm::rotate((3.1416f / 2), rotAxis) * glm::normalize(glm::vec4(offsetFromCenterOfMass, 1.0))));
        newPhysics.rotationalAcceleration += glm::vec4(rotAxis, (impulse * projCoef) / (RigidBody::mass() * delta * glm::length(offsetFromCenterOfMass)));
    }

    setPhysics(newPhysics);
    //setTransformation(newLocation);
}
