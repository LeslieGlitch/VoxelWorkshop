/*
 * Voxel.cpp
 * 
 * Implementation file for the Voxel class
 */

#include "Voxel.h"

Voxel::Voxel(Material mat) : material(mat), active(true)
{}

Voxel::~Voxel() {
	return;
}

void Voxel::isActive(bool isActive) {
	active = isActive;
	return;
}

bool Voxel::isActive() {
	return active;
}
