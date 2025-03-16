/**
 *  VisualData.h
 * 
 *  Storage class for visual information of a voxel
 */

#ifndef VISUALDATA_H
#define VISUALDATA_H

#include <vector>

class VisualData {
private:
    std::vector<unsigned char> color;
    std::vector<signed char> normalVector;
};

#endif
