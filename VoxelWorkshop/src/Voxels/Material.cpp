/*
 * Material.cpp
 *
 * Implementation file for Material class
 */

#include "Material.h"

glm::vec3 MatColors[] = {
    glm::vec3(0.82, 0.82, 0.82), // stone
    glm::vec3(0.77, 0.62, 0.43), // wood,
    glm::vec3(0.13, 0.55, 0.13), // foliage
    glm::vec3(0.6 , 0.6 , 0.6 ), // iron
    glm::vec3(1.0 , 0.84, 0.0 ), // gold
    glm::vec3(0.94, 0.90, 0.55), // cloth
    glm::vec3(0.55, 0.27, 0.08), // dirt
    glm::vec3(0.68, 0.85, 0.90)  // clay
};

// density of each material (in g/m^3)
float MatDensities[] = {
    2.25, // stone
    1.00, // wood
    1.00, // foliage
    1.00, // iron
    1.00, // gold
    1.00, // cloth
    1.00, // dirt
    1.00  // clay
};

Material::Material(MatList ID) {
    Material::ID = ID;
}

Material::Material(const Material& material) {
    Material::ID = material.ID;
}

glm::vec3 Material::getColor() const {
    return MatColors[ID];
}

void Material::setMaterial(MatList ID) {
    Material::ID = ID;
}

MatList Material::getMaterial() {
    return Material::ID;
}

float Material::getDensity() const {
    return MatDensities[ID] * 1000;
}
