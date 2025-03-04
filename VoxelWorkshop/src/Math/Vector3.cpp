#include "Vector3.h"
#include <cmath>

Math::Vector3<int> i = Math::Vector3<int>(1, 0, 0);
Math::Vector3<int> j = Math::Vector3<int>(0, 1, 0);
Math::Vector3<int> k = Math::Vector3<int>(0, 0, 1);

// constructors
template<class T>
Math::Vector3<T>::Vector3(T x, T y, T z) : xVal(x), yVal(y), zVal(z) {}
template<class T>
Math::Vector3<T>::Vector3(const Math::Vector3<T> &orig) : xVal(orig.x()), yVal(orig.y()), zVal(orig.z()) {}

/* Operators */

/// @brief Normalize the vector to a length of 1
template<class T>
void Math::Vector3<T>::normalize() {
	T vectorLength = sqrt((xVal * xVal) + (yVal * yVal) + (zVal * zVal));
	xVal /= vectorLength;
	yVal /= vectorLength;
	zVal /= vectorLength;
}

/// @brief Cross product
/// @param right Right side of operation
/// @return Cross product of this vector * right vector
template<class T>
Math::Vector3<T> Math::Vector3<T>::cross(const Math::Vector3<T>& right) const {
	T a = (this->y() * right.z()) - (this->z() * right.y());
	T b = (this->z() * right.x()) - (this->x() * right.z());
	T c = (this->x() * right.y()) - (this->y() * right.x());
	return Math::Vector3(a, b, c);
}

/// @brief Dot product
/// @param right Right side of operation
/// @return Dot product of this vector * right vector
template<class T>
T Math::Vector3<T>::dot(const Math::Vector3<T>& right) const {
	return (this->x() * right.x()) + (this->y() * right.y()) + (this->z() * right.z());
}

template<class T>
Math::Vector3<T> Math::Vector3<T>::operator*(const T& right) {
	return Math::Vector3<T>(this->x()*right, this->y()*right, this->z()*right);
}

template<class T>
Math::Vector3<T> Math::Vector3<T>::operator+(const Math::Vector3<T>& right) {
	return Math::Vector3((this->x() + right.x()), (this->y() + right.y()), (this->z() + right.z()));
}

template<class T>
Math::Vector3<T> Math::Vector3<T>::operator-(const Math::Vector3<T>& right) {
	return Math::Vector3((this->x() - right.x()), (this->y() - right.y()), (this->z() - right.z()));
}

// setters
template<class T>
void Math::Vector3<T>::x(T val) { xVal = val; }
template<class T>
void Math::Vector3<T>::y(T val) { yVal = val; }
template<class T>
void Math::Vector3<T>::z(T val) { zVal = val; }
template<class T>
void Math::Vector3<T>::r(T val) { xVal = val; }
template<class T>
void Math::Vector3<T>::g(T val) { yVal = val; }
template<class T>
void Math::Vector3<T>::b(T val) { zVal = val; }

// getters
template<class T>
T Math::Vector3<T>::x() const { return xVal; }
template<class T>
T Math::Vector3<T>::y() const { return yVal; }
template<class T>
T Math::Vector3<T>::z() const { return zVal; }
template<class T>
T Math::Vector3<T>::r() const { return xVal; }
template<class T>
T Math::Vector3<T>::g() const { return yVal; }
template<class T>
T Math::Vector3<T>::b() const { return zVal; }
