#pragma once

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

int32 fastfloor(float x);
int32 fastfloor(double x);
int32 min32(int32 a, int32 b);

class Vector {
public:
	Vector(Vector *s);
	Vector(double x0, double y0, double z0);

	void set(Vector *o);
	void set(double x0, double y0, double z0);

	void add(Vector *o);	
	void add(double x0, double y0, double z0);

	void sub(Vector *o);
	void sub(double x0, double y0, double z0);

	void mul(double x0);
	void mul(double x0, double y0, double z0);

	double lengthSquared();
	double length();
	void normalize();

	double dot(Vector *o);
	double dot(double ox, double oy, double oz);

	double distSquared(Vector *o);
	double distSquared(float ox, float oy, float oz);

	double x, y, z;

};

Vector cross(Vector *a, Vector *b);


enum Axis {
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
};

Axis largestAxis(double x, double y, double z);
