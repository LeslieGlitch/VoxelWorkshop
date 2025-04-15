/*
 * RigidBody.cpp
 *
 * Implementation file for Rigid Body class
 */

#include "RigidBody.h"

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
    RigidBody::location.Position += RigidBody::movement.linearVelocity;
    RigidBody::movement.linearVelocity += RigidBody::movement.linearAcceleration;

    // Update rotation data
    RigidBody::location.Rotation += RigidBody::movement.rotationalVelocity;
    RigidBody::movement.rotationalVelocity += RigidBody::movement.rotationalAcceleration;
}
