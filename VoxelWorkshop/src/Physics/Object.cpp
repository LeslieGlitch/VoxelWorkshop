/*
 * Object.cpp
 * 
 * Implementation class for base physics object
 */

#include "Object.h"
#include <glm/glm.hpp>

unsigned int indexArraySize;

Object::Object() {
    Object::structure.loadFromFile("region0.txt");
    indexArraySize = Object::structure.generateMesh(Object::location.Position);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

Object::Object(Brickmap structure) {
    Object::structure = structure;
}

void Object::setPosition(glm::vec3 position) {
    Object::location.Position = position;
    indexArraySize = Object::structure.generateMesh(position);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

void Object::render() {
    Object::structure.VAO.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, indexArraySize, GL_UNSIGNED_INT, 0);
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
