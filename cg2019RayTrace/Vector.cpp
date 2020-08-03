#include "stdafx.h"
#include "Vector.h"

#include <cmath>

int32 fastfloor(float x)
{
	int32 xi = (int32)x;
	return x < xi ? xi - 1 : xi;
};

int32 fastfloor(double x)
{
	int32 xi = (int32)x;
	return x < xi ? xi - 1 : xi;
};

int32 min32(int32 a, int32 b)
{
	return a < b ? a : b;
}

Vector::Vector(Vector *s) {
	x = s->x;
	y = s->y;
	z = s->z;
}

Vector::Vector(double x0, double y0, double z0)
{
	x = x0;
	y = y0;
	z = z0;
}

void Vector::set(double x0, double y0, double z0) {
	x = x0;
	y = y0;
	z = z0;
}

void Vector::set(Vector *o) {
	x = o->x;
	y = o->y;
	z = o->z;
}

void Vector::add(double x0, double y0, double z0) {
	x += x0;
	y += y0;
	z += z0;
}

void Vector::add(Vector *o) {
	x += o->x;
	y += o->y;
	z += o->z;
}

void Vector::sub(double x0, double y0, double z0) {
	x -= x0;
	y -= y0;
	z -= z0;
}

void Vector::sub(Vector *o) {
	x -= o->x;
	y -= o->y;
	z -= o->z;
}

void Vector::mul(double x0) {
	x *= x0;
	y *= x0;
	z *= x0;
}

void Vector::mul(double x0, double y0, double z0) {
	x *= x0;
	y *= y0;
	z *= z0;
}

double Vector::lengthSquared() {
	return x * x + y * y + z * z;
}

double Vector::length() {
	return std::sqrt(x * x + y * y + z * z);
}

void Vector::normalize() {
	double len = length();
	x /= len;
	y /= len;
	z /= len;
}

double Vector::dot(Vector *o) {
	return x * o->x + y * o->y + z * o->z;
}

double Vector::dot(double ox, double oy, double oz) {
	return x * ox + y * oy + z * oz;
}

double Vector::distSquared(Vector *o) {
	return (x - o->x) * (x - o->x) + (y - o->y) * (y - o->y) + (z - o->z) * (z - o->z);
}

double Vector::distSquared(float ox, float oy, float oz) {
	return (x - ox) * (x - ox) + (y - oy) * (y - oy) + (z - oz) * (z - oz);
}

Vector cross(Vector *a, Vector *b)
{
	Vector result(a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x);
	return result;
}

Axis largestAxis(double x, double y, double z)
{
	if (x > y) {
		if (x > z) {
			return X_AXIS;
		}
		else {
			return Z_AXIS;
		}
	}
	else {
		if (y > z) {
			return Y_AXIS;
		}
		else {
			return Z_AXIS;
		}
	}
}