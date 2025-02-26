/*
 * Vector3.h
 *
 * Header file for the chunk class
 */

#ifndef MATRIX4_H
#define MATRIX4_H

namespace Math {
	class Matrix4 {
	public:
		float m[16];

		// operations
		void identity();
	};
}

#endif
