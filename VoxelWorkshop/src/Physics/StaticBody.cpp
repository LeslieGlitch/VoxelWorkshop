/* 
 * StaticBody.cpp
 * 
 * Implementation file for Static Body class
 */

#include "StaticBody.h"

StaticBody::StaticBody() {
    StaticBody::structure.loadFromFile("hSphere.bm");
    indexArraySize = StaticBody::structure.generateMesh(StaticBody::location, StaticBody::material);
    // Link mesh to the VAO
    StaticBody::structure.linkMesh();
}

StaticBody::StaticBody(std::string filename) {
    StaticBody::structure.loadFromFile(filename);
    indexArraySize = StaticBody::structure.generateMesh(StaticBody::location, StaticBody::material);
    // Link mesh to the VAO
    StaticBody::structure.linkMesh();
}

StaticBody::StaticBody(Brickmap structure) {
    StaticBody::structure = structure;
}

void StaticBody::start() {
    // Perform Base start logic
    Object::start();
    return;
}

void StaticBody::update() {
    // Perform Base update logic
    Object::update();
    return;
}

std::string StaticBody::getType() {
    return "Static";
}
