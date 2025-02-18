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
	/// @param material Material for the current voxel, defaults to Vacuum
	Voxel(Material material = Material::Vacuum);

	/// @brief Destructor for a single voxel
	~Voxel();

	/// @brief Returns whether the voxel has anything or not
	/// @return True if the voxel is active, False otherwise
	bool isActive();

	/// @brief Sets the active state of the voxel
	/// @param active State to set voxel
	void isActive(bool active);

private:
	bool active;
	Material material;
};

#endif
