/*
 * Object.h
 * 
 * Include file for base physics object
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "../Voxels/Brickmap.h"
#include "../Voxels/Material.h"

struct PhysicsData {
    glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 rotationalVelocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 rotationalAcceleration = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class Object {
public:
    // Where the object is and is going
    LocationData location;
    PhysicsData movement;

    // Object Material
    Material material = Material(stone);

    // The brickmap model
    Brickmap structure;

    // Index array size for rendering
    unsigned int indexArraySize = 0;

    /// @brief Default Constructor
    Object();

    /// @brief Constructor with brickmap filename
    Object(std::string filename);

    /// @brief Contructor with Brickmap
    Object(Brickmap structure);

    /// @brief Updates the object position and regenerates the mesh
    /// @param position new position for the object
    void setTransformation(const LocationData& location);

    void setMaterial(const Material& material);

    /// @brief Renders the game object
    void render();

    /// @brief Function called when object spawns
    void start();

    /// @brief Function called once every game tick
    void update();
};

#endif
