/*
 * Material.h
 * 
 * Include file for Material types
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

class Material {
private:
    std::string ID = "";
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

public:
    Material(std::string ID, glm::vec3 color) { Material::ID = ID; Material::color = color; }
    Material(const Material& material) { Material::ID = material.ID; Material::color = material.color; }
    glm::vec3 getColor() const { return color; }
};

#endif
