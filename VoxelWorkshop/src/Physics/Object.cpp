/*
 * Object.cpp
 * 
 * Implementation class for base physics object
 */

#include "Object.h"
#include <glm/glm.hpp>

Object::Object(Brickmap structure) {
    Object::structure = structure;
}

void Object::render() {
    Object::structure.generateMesh(Object::location.Position);
}

void Object::start() {
    return;
}

void Object::update() {
    // Update location data
    Object::location.Position += Object::movement.linearVelocity;
    Object::movement.linearVelocity += Object::movement.linearAcceleration;

    // Update rotation data
    Object::location.Rotation += Object::movement.rotationalVelocity;
    Object::movement.rotationalVelocity += Object::movement.rotationalAcceleration;
}
