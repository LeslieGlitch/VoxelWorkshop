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

RigidBody::RigidBody(const RigidBody& source) {
    RigidBody::location = source.location;
    RigidBody::movement = source.movement;
    RigidBody::material = source.material;
    RigidBody::structure = source.structure;
    RigidBody::indexArraySize = source.indexArraySize;
}

RigidBody RigidBody::operator=(const RigidBody& right) {
    RigidBody result;
    result.location = right.location;
    result.movement = right.movement;
    result.material = right.material;
    result.structure = right.structure;
    result.indexArraySize = right.indexArraySize;
    return result;
}

void RigidBody::start() {
    // Perform Base start logic
    indexArraySize = RigidBody::structure.generateMesh(RigidBody::location, RigidBody::material);
    // Link mesh to the VAO
    RigidBody::structure.linkMesh();
    Object::start();
    return;
}

void RigidBody::update() {
    if (!canUpdate) {
        return;
    }

    // Perform Base update logic
    Object::update();

    // Update location data
    LocationData newLocation = RigidBody::location;
    PhysicsData newPhysics = RigidBody::movement;
    newLocation.Position += RigidBody::movement.linearVelocity * delta;
    newPhysics.linearVelocity += RigidBody::movement.linearAcceleration * delta;

    // Update rotation data
    newLocation.Rotation = glm::angleAxis(glm::angle(RigidBody::movement.rotationalVelocity) * delta, glm::axis(RigidBody::movement.rotationalVelocity)) * newLocation.Rotation;
    newPhysics.rotationalVelocity = glm::angleAxis(glm::angle(RigidBody::movement.rotationalAcceleration) * delta, glm::axis(RigidBody::movement.rotationalAcceleration)) * newPhysics.rotationalVelocity;
    
    setPhysics(newPhysics);
    setTransformation(newLocation);

    return;
}

bool isMovingApart(const Object& self, const Object& collider) {
    glm::vec3 relPos = self.location.Position - collider.location.Position;
    glm::vec3 relVel = self.movement.linearVelocity - collider.movement.linearVelocity;
    return glm::dot(relPos, relVel) > 0.0;
}

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
        return;
    }
    // find center of collision
    glm::vec3 centerOfCollision(0, 0, 0);
    for (int i = 0; i < 14 * 14 * 14; ++i) {
        if (rotatedSelf[i]) centerOfCollision += this->structure.indexToCoords(i, 14);
    }
    centerOfCollision /= rotatedSelf.count();
    centerOfCollision -= glm::vec3(7.0, 7.0, 7.0);

    if (collider.getType() == "Rigid") {
        this->Impulse(collider.mass() * glm::length(collider.movement.linearVelocity - this->movement.linearVelocity), glm::normalize(collider.movement.linearVelocity - this->movement.linearVelocity), centerOfCollision);
    }
    else {
        // replace mass with self
        this->Impulse(1.5f * this->mass() * glm::length(collider.movement.linearVelocity - this->movement.linearVelocity), glm::normalize(collider.movement.linearVelocity - this->movement.linearVelocity), centerOfCollision);
    }
    
    //collisionCooldown = 5 * ceil(log(glm::length(this->movement.linearVelocity)));
}

std::string RigidBody::getType() {
    return "Rigid";
}

void RigidBody::applyDirectionGravity(glm::vec3 direction) {
    // Apply gravitational force
    RigidBody::Impulse(RigidBody::mass() * 9.8f * delta, direction);
    return;
}

void RigidBody::applyPointGravity(glm::vec3 pointLocation) {
    // Apply gravitational force
    RigidBody::Impulse(RigidBody::mass() * 9.8f * delta, pointLocation - RigidBody::location.Position);
    return;
}

#include <iostream>
void RigidBody::Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass) {
    // Guard clause: no direction
    if (glm::length(direction) < 0.1) {
        return;
    }

    // Create data to emplace
    //LocationData newLocation = RigidBody::location;
    PhysicsData newPhysics = RigidBody::movement;

    // Update linear acceleration
    newPhysics.linearAcceleration += glm::normalize(direction) * (impulse / (RigidBody::mass() * delta));

    // Update rotation acceleration
    if (glm::length(offsetFromCenterOfMass) > 0.1) {
        std::cout << "oldRot: (" << newPhysics.rotationalAcceleration.w << ", " << newPhysics.rotationalAcceleration.x << ", " << newPhysics.rotationalAcceleration.y << ", " << newPhysics.rotationalAcceleration.z << ")\n";
        std::cout << "Rotation\n";
        glm::vec3 rotAxis = glm::normalize(glm::cross(offsetFromCenterOfMass, direction));
        float projCoef = glm::dot(glm::normalize(direction), glm::vec3(glm::rotate((3.1416f / 2), rotAxis) * glm::normalize(glm::vec4(offsetFromCenterOfMass, 1.0))));
        float accMagnitude = (impulse * projCoef * glm::length(offsetFromCenterOfMass)) / (RigidBody::mass() * delta);
        newPhysics.rotationalAcceleration = glm::angleAxis(glm::degrees(accMagnitude), rotAxis) * newPhysics.rotationalAcceleration;
        std::cout << "newRot: (" << newPhysics.rotationalAcceleration.w << ", " << newPhysics.rotationalAcceleration.x << ", " << newPhysics.rotationalAcceleration.y << ", " << newPhysics.rotationalAcceleration.z << ")\n";
    }

    setPhysics(newPhysics);
    //setTransformation(newLocation);
}
