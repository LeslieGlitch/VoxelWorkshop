/*
 * Voxel.h
 * 
 * Header file for the voxel class
 */

#ifndef VOXEL_H
#define VOXEL_H

enum Material {
    Vacuum,
    Stone,
    Dirt,
    Foliage,
};

class Voxel {
public:
    /// @brief Constructor for a single voxel
    /// @param voxelColor albedo color for the current voxel, defaults to White
    Voxel(int voxelColor[3]);

    /// @brief Default constructor for a single voxel
    Voxel();

    /// @brief Destructor for a single voxel
    ~Voxel();

    /// @brief Returns whether the voxel has anything or not
    /// @return True if the voxel is active, False otherwise
    bool isActive() const;

    /// @brief Sets the active state of the voxel
    /// @param active State to set voxel
    void isActive(bool active);

    /// @brief Gets the albedo color of the voxel
    /// @return An array of 3 integers (0-255) in the form {r, g, b}
    int* getColor() const;

    /// @brief Sets the albedo color of the voxel
    /// @param albedo Array of 3 integers (0-255) in the form {r, g, b}
    void setColor(int albedo[3]);

    // overloaded operators
    Voxel operator=(Voxel const& right);

private:
    bool active;
    int color[3];
    //Material material;
};

#endif
