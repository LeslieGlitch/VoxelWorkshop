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

StaticBody::StaticBody(const StaticBody& source) {
    StaticBody::location = source.location;
    StaticBody::movement = source.movement;
    StaticBody::material = source.material;
    StaticBody::structure = source.structure;
    StaticBody::indexArraySize = source.indexArraySize;
}

StaticBody StaticBody::operator=(const StaticBody& right) {
    StaticBody result;
    result.location = right.location;
    result.movement = right.movement;
    result.material = right.material;
    result.structure = right.structure;
    result.indexArraySize = right.indexArraySize;
    return result;
}

void StaticBody::start() {
    indexArraySize = StaticBody::structure.generateMesh(StaticBody::location, StaticBody::material);
    // Link mesh to the VAO
    StaticBody::structure.linkMesh();

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
