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
    glm::vec3(0.27, 0.14, 0.04), // dirt
    glm::vec3(0.68, 0.85, 0.90)  // clay
};

// density of each material (in kg/m^3)
float MatDensities[] = {
    2250.00, // stone
    750.000, // wood
    0.05000, // foliage
    7870.00, // iron
    19300.0, // gold
    0.00154, // cloth
    1220.00, // dirt
    1600.00  // clay
};

Material::Material() {
    Material::ID = stone;
}

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
    return MatDensities[ID];
}
