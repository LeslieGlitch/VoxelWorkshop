/**
 *  Brickgrid.h
 *
 *  Header file for brickgrid class
 *  Brickmap voxel structure invented by Thijs van Wingerden https://studenttheses.uu.nl/handle/20.500.12932/20460
 */

#ifndef BRICKGRID_H
#define BRICKGRID_H

#include "Brickmap.h"

const int BRICKGRID_SIZE = 128;

class Brickgrid {
private:
    Brickmap* __ptr32 grid[BRICKGRID_SIZE * BRICKGRID_SIZE * BRICKGRID_SIZE];
};

#endif
