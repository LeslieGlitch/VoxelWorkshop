/**
 *  VisualData.h
 * 
 *  Storage class for visual information of a voxel
 */

#ifndef VISUALDATA_H
#define VISUALDATA_H

#include "VisualData.h"
#include "../Math/Vector3.h"

class VisualData {
private:
	Math::Vector3<unsigned char> color;
	Math::Vector3<signed char> normalVector;
};

#endif
