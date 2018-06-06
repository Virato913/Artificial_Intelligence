#pragma once

#include <math.h>

#define PI atan(1)*4
#define RAD(c) (PI*c)/180

struct Vector2
{
	double x, y;

	Vector2()
	{
		this->x = 0;
		this->y = 0;
	}

	Vector2(double a, double b)
	{
		this->x = a;
		this->y = b;
	}

	Vector2 operator+(Vector2 other)
	{
		Vector2 pivot;
		pivot.x = this->x + other.x;
		pivot.y = this->y + other.y;
		return pivot;
	}

	void operator+=(Vector2 other)
	{
		this->x += other.x;
		this->y += other.y;
	}

	Vector2 operator-(Vector2 other)
	{
		Vector2 pivot;
		pivot.x = this->x - other.x;
		pivot.y = this->y - other.y;
		return pivot;
	}

	Vector2 operator*(double times)
	{
		Vector2 pivot;
		pivot.x = this->x*times;
		pivot.y = this->y*times;
		return pivot;
	}

	void operator*=(double times)
	{
		this->x *= times;
		this->y *= times;
	}

	void operator/=(float times)
	{
		this->x /= times;
		this->y /= times;
	}

	void operator=(Vector2 other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	bool operator==(Vector2 other)
	{
		return (this->x == other.x && this->y == other.y);
	}

	bool operator!=(Vector2 other)
	{
		return !(*this == other);
	}

	void Normalize()
	{
		Vector2 pivot;
		pivot.x = this->x / this->Lenght();
		pivot.y = this->y / this->Lenght();
		*this = pivot;
	}

	Vector2 Normalized()
	{
		Vector2 pivot = *this;
		pivot.Normalize();
		return pivot;
	}

	double Lenght()
	{
		return sqrt((pow(x, 2) + pow(y, 2)));
	}

	double GetAngle()
	{
		double pivot = atan(this->y / this->x);
		if (this->x < 0)
		{
			pivot += PI;
		}
		if (this->x >= 0 && this->y < 0)
		{
			pivot += 2 * PI;
		}
		return pivot;
	}
};