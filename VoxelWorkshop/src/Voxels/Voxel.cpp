/*
 * Voxel.cpp
 * 
 * Implementation file for the Voxel class
 */

#include "Voxel.h"

Voxel::Voxel(int voxelColor[3]) : active(false) {
	setColor(voxelColor);
}

Voxel::Voxel() : active(false) {
	int albedo[3] = { 255, 255, 255 };
	setColor(albedo);
}

Voxel::~Voxel() {
	active = false;
	return;
}

void Voxel::isActive(bool isActive) {
	active = isActive;
	return;
}

bool Voxel::isActive() const {
	return active;
}

int* Voxel::getColor() const {
	int albedo[3] = {color[0], color[1], color[2]};
	return albedo;
}

void Voxel::setColor(int albedo[3]) {
	for (int i = 0; i < 3; ++i) {
		color[i] = albedo[i];
	}
}

Voxel Voxel::operator=(const Voxel& right) {
	Voxel result;
	active = right.isActive();
	setColor(right.getColor());
	return result;
}
