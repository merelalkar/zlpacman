#ifndef PEGAS_VECTORS_H
#define PEGAS_VECTORS_H
#pragma once

namespace Pegas
{
	class Vector3
	{
	public:
		float _x, _y, _z;

		Vector3();
		Vector3(const Vector3& src);
		Vector3(float x, float y, float z);

		float length();
		
		friend Vector3 operator+(const Vector3& a, const Vector3& b);
		friend Vector3 operator-(const Vector3& a, const Vector3& b);
		friend Vector3 operator*(float a, const Vector3& b);
		friend Vector3 operator*(const Vector3& a, float b);
		friend Vector3 operator/(const Vector3& a, float b);
	};

	inline Vector3::Vector3(): _x(0), _y(0), _z(0)
	{

	}

	inline Vector3::Vector3(const Vector3& src): _x(src._x), _y(src._y), _z(src._z)
	{

	}

	inline Vector3::Vector3(float x, float y, float z)
		: _x(x), _y(y), _z(z)
	{

	}

	inline Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3((a._x + b._x), (a._y + b._y), (a._z + b._z));
	}

	inline Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3((a._x - b._x), (a._y - b._y), (a._z - b._z));
	}

	inline Vector3 operator*(float a, const Vector3& b)
	{
		return Vector3((a * b._x), (a * b._y), (a * b._z));
	}

	inline Vector3 operator*(const Vector3& a, float b)
	{
		return Vector3((a._x * b), (a._y * b), (a._z * b));
	}

	inline Vector3 operator/(const Vector3& a, float b)
	{
		return Vector3((a._x / b), (a._y / b), (a._z / b));
	}

	inline float Vector3::length()
	{
		return sqrt((_x * _x) + (_y * _y) + (_z * _z));
	}
}

#endif//PEGAS_VECTORS_H