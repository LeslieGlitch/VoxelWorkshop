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

public:
    Material();
    Material(MatList ID);
    Material(const Material& material);
    glm::vec3 getColor() const;
    void setMaterial(MatList ID);
    MatList getMaterial();

    /// @brief Get the density of the material
    /// @return Density in kg/(m^3)
    float getDensity() const;
};

#endif
