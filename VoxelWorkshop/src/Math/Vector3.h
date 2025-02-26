/*
 * Vector3.h
 *
 * Header file for the chunk class
 */

#ifndef VECTOR3_H
#define VECTOR3_H

namespace Math {
	class Vector3 {
	private:
		float xVal;
		float yVal;
		float zVal;

	public:
		// constructors
		Vector3(float x, float y, float z);
		Vector3(const Vector3 &orig);

		// operators
		void normalize();
		Vector3 cross(const Vector3& right) const;
		float dot(const Vector3& right) const;
		Vector3 operator*(const float& right);
		Vector3 operator+(const Vector3& right);
		Vector3 operator-(const Vector3& right);

		// setters
		void x(float val);
		void y(float val);
		void z(float val);
		void r(float val);
		void g(float val);
		void b(float val);

		// getters
		float x() const;
		float y() const;
		float z() const;
		float r() const;
		float g() const;
		float b() const;
	};
}

#endif
