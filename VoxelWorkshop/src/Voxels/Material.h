/*
 * Material.h
 * 
 * Include file for Material types
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

enum MatList {
    stone,
    wood,
    foliage,
    iron,
    gold,
    cloth,
    dirt,
    clay
};

class Material {
private:
    MatList ID = stone;
    glm::vec3 color = glm::vec3(0.82, 0.82, 0.82);

public:
    Material(MatList ID);
    Material(const Material& material);
    glm::vec3 getColor() const;
    void setMaterial(MatList ID);
    MatList getMaterial();
};

#endif
