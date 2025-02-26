#include "Vector3.h"
#include <cmath>

Math::Vector3 i = Math::Vector3(1, 0, 0);
Math::Vector3 j = Math::Vector3(0, 1, 0);
Math::Vector3 k = Math::Vector3(0, 0, 1);

// constructors
Math::Vector3::Vector3(float x, float y, float z) : xVal(x), yVal(y), zVal(z) {}
Math::Vector3::Vector3(const Math::Vector3 &orig) : xVal(orig.x()), yVal(orig.y()), zVal(orig.z()) {}

/* Operators */

/// @brief Normalize the vector to a length of 1
void Math::Vector3::normalize() {
	float vectorLength = sqrt((xVal * xVal) + (yVal * yVal) + (zVal * zVal));
	xVal /= vectorLength;
	yVal /= vectorLength;
	zVal /= vectorLength;
}

/// @brief Cross product
/// @param right Right side of operation
/// @return Cross product of this vector * right vector
Math::Vector3 Math::Vector3::cross(const Math::Vector3& right) const {
	float a = (this->y() * right.z()) - (this->z() * right.y());
	float b = (this->z() * right.x()) - (this->x() * right.z());
	float c = (this->x() * right.y()) - (this->y() * right.x());
	return Math::Vector3(a, b, c);
}

/// @brief Dot product
/// @param right Right side of operation
/// @return Dot product of this vector * right vector
float Math::Vector3::dot(const Math::Vector3& right) const {
	return (this->x() * right.x()) + (this->y() * right.y()) + (this->z() * right.z());
}

Math::Vector3 Math::Vector3::operator*(const float& right) {
	return Math::Vector3(this->x()*right, this->y()*right, this->z()*right);
}

Math::Vector3 Math::Vector3::operator+(const Math::Vector3& right) {
	return Math::Vector3((this->x() + right.x()), (this->y() + right.y()), (this->z() + right.z()));
}

Math::Vector3 Math::Vector3::operator-(const Math::Vector3& right) {
	return Math::Vector3((this->x() - right.x()), (this->y() - right.y()), (this->z() - right.z()));
}

// setters
void Math::Vector3::x(float val) { xVal = val; }
void Math::Vector3::y(float val) { yVal = val; }
void Math::Vector3::z(float val) { zVal = val; }
void Math::Vector3::r(float val) { xVal = val; }
void Math::Vector3::g(float val) { yVal = val; }
void Math::Vector3::b(float val) { zVal = val; }

// getters
float Math::Vector3::x() const { return xVal; }
float Math::Vector3::y() const { return yVal; }
float Math::Vector3::z() const { return zVal; }
float Math::Vector3::r() const { return xVal; }
float Math::Vector3::g() const { return yVal; }
float Math::Vector3::b() const { return zVal; }
