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
    newLocation.Rotation = glm::rotate(RigidBody::movement.rotationalVelocity.w * delta, glm::vec3(RigidBody::movement.rotationalVelocity.x, RigidBody::movement.rotationalVelocity.y, RigidBody::movement.rotationalVelocity.z)) * newLocation.Rotation;
    newPhysics.rotationalVelocity = glm::rotate(RigidBody::movement.rotationalAcceleration.w * delta, glm::vec3(RigidBody::movement.rotationalAcceleration.x, RigidBody::movement.rotationalAcceleration.y, RigidBody::movement.rotationalAcceleration.z)) * newPhysics.rotationalVelocity;
    newPhysics.rotationalAcceleration = glm::vec4(0.0, 0.0, 1.0, 0.0);

    setPhysics(newPhysics);
    setTransformation(newLocation);

    return;
}

bool isMovingApart(const Object& self, const Object& collider) {
    glm::vec3 relPos = self.location.Position - collider.location.Position;
    glm::vec3 relVel = self.movement.linearVelocity - collider.movement.linearVelocity;
    return glm::dot(relPos, relVel) > 0.0;
}

#include <iostream>
void RigidBody::detectCollision(const Object& collider) {
    if (collisionCooldown > 0) {
        // too soon after last collision
        return;
    }

    if (glm::distance(Object::location.Position, collider.location.Position) > 1.75) {
        // collision not possible
        return;
    }

    if (isMovingApart(*this, collider)) {
        // don't collide objects already moving away from one another
        return;
    }

    std::cout << "Objects are close enough to possible collide\n";

    // get collider position relative to self
    glm::ivec3 offset = (collider.location.Position - this->location.Position);
    offset *= 0.125;

    // create bitset big enough to hold rotated object
    std::bitset <14 * 14 * 14> rotatedSelf = this->getRotatedStructure(glm::ivec3(0, 0, 0));
    std::bitset<14 * 14 * 14> rotatedCollider = collider.getRotatedStructure(offset);

    //int origCount = this->structure.voxelCount();
    //int newCount = rotatedSelf.count();

    // AND the two bitsets together, result is overlap
    rotatedSelf &= rotatedCollider;

    if (rotatedSelf.count() == 0) {
        // no collision
        std::cout << "found no collision\n\n";
        return;
    }
    std::cout << "found collision\n\n";
    // find center of collision
    glm::vec3 centerOfCollision(0, 0, 0);
    for (int i = 0; i < 14 * 14 * 14; ++i) {
        if (rotatedSelf[i]) centerOfCollision += this->structure.indexToCoords(i, 14);
    }
    centerOfCollision /= rotatedSelf.count();
    centerOfCollision -= glm::vec3(7.0, 7.0, 7.0);

    this->Impulse(collider.mass() * glm::length(collider.movement.linearVelocity - this->movement.linearVelocity), glm::normalize(collider.movement.linearVelocity - this->movement.linearVelocity), centerOfCollision);
    
    collisionCooldown = 5 * ceil(log(glm::length(this->movement.linearVelocity)));
}

std::string RigidBody::getType() {
    return "Rigid";
}

void RigidBody::Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass) {
    // Create data to emplace
    //LocationData newLocation = RigidBody::location;
    PhysicsData newPhysics = RigidBody::movement;

    // Update linear acceleration
    newPhysics.linearAcceleration += glm::normalize(direction) * (impulse / (RigidBody::mass() * delta));

    // Update rotation acceleration
    if (glm::length(offsetFromCenterOfMass) > 0.1) {
        glm::vec3 rotAxis = glm::normalize(glm::cross(offsetFromCenterOfMass, direction));
        float projCoef = glm::dot(glm::normalize(direction), glm::vec3(glm::rotate((3.1416f / 2), rotAxis) * glm::normalize(glm::vec4(offsetFromCenterOfMass, 1.0))));
        newPhysics.rotationalAcceleration = glm::rotate((impulse * projCoef) / (RigidBody::mass() * delta * glm::length(offsetFromCenterOfMass)), rotAxis) * newPhysics.rotationalAcceleration;
    }

    setPhysics(newPhysics);
    //setTransformation(newLocation);
}
