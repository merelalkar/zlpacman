#ifndef PEGAS_VECTORS_H
#define PEGAS_VECTORS_H
#pragma once

namespace Pegas
{
	class Vector3
	{
	public:
		double _x, _y, _z;

		Vector3();
		Vector3(double x, double y, double z);

		friend Vector3 operator+(const Vector3& a, const Vector3& b);
		friend Vector3 operator-(const Vector3& a, const Vector3& b);
		friend Vector3 operator*(double a, const Vector3& b);
		friend Vector3 operator*(const Vector3& a, double b);
		friend Vector3 operator/(const Vector3& a, double b);
	};

	inline Vector3::Vector3(): _x(0), _y(0), _z(0)
	{

	}

	inline Vector3::Vector3(double x, double y, double z)
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

	inline Vector3 operator*(double a, const Vector3& b)
	{
		return Vector3((a * b._x), (a * b._y), (a * b._z));
	}

	inline Vector3 operator*(const Vector3& a, double b)
	{
		return Vector3((a._x * b), (a._y * b), (a._z * b));
	}

	inline Vector3 operator/(const Vector3& a, double b)
	{
		return Vector3((a._x / b), (a._y / b), (a._z / b));
	}
}

#endif//PEGAS_VECTORS_H