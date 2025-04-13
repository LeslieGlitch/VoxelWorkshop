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
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

Object::Object(Brickmap structure) {
    Object::structure = structure;
}

void Object::setTransformation(const LocationData& location) {
    Object::location = location;
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

void Object::setMaterial(const Material& material) {
    Object::material = material;
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
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
