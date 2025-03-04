/*
 * Vector3.h
 *
 * Header file for the chunk class
 */

#ifndef VECTOR3_H
#define VECTOR3_H

namespace Math {
	template<class T>
	class Vector3 {
	private:
		T xVal;
		T yVal;
		T zVal;

	public:
		// constructors
		Vector3(T x, T y, T z);
		Vector3(const Vector3 &orig);

		// operators
		void normalize();
		Vector3 cross(const Vector3<T>& right) const;
		T dot(const Vector3<T>& right) const;
		Vector3 operator*(const T& right);
		Vector3 operator+(const Vector3<T>& right);
		Vector3 operator-(const Vector3<T>& right);

		// setters
		void x(T val);
		void y(T val);
		void z(T val);
		void r(T val);
		void g(T val);
		void b(T val);

		// getters
		T x() const;
		T y() const;
		T z() const;
		T r() const;
		T g() const;
		T b() const;
	};
}

#endif
