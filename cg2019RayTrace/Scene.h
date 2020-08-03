#pragma once

#include "Vector.h"

class SceneObject 
{
public:

	virtual bool intersect(Vector *ray_source, Vector *ray, Vector *result, Vector *result_normal, double dt) = 0;

	double x, y, z;
	float red, green, blue;

	double absorb_chance;
	double diffuse_chance;
	double specular_chance;
	double transmission_chance;
	double refraction;
	double specular_coeff;
};

class CCgScene
{
public:
	CCgScene(int32 num_objects);
	~CCgScene();

	int32 size;
	SceneObject **objects;

	void intersect(Vector &ray_source, Vector &ray, SceneObject *exclude, Vector *result, Vector *result_normal, SceneObject **hit_object, double dt);

};

// A spherical object in the scene
class SphereObject : public SceneObject
{
public:
	SphereObject(double x0, double y0, double z0, double r0, uint32 col, double d, double s, double t, double a);
	SphereObject(double x0, double y0, double z0, double r0, uint32 col, double d, double s, double t, double a, double dx, double dy, double dz);

	bool intersect(Vector *ray_source, Vector *ray, Vector *result, Vector *result_normal, double dt) override;

	double radius;
	double dx, dy, dz;
};

// A planar object, although a litle specialized to create the walls of a cornell box
class PlaneObject : public SceneObject {
public:
	// x0,y0,z0 should form a unit vector in the axis of the plane, arbitrary planes not supported
	PlaneObject(double x0, double y0, double z0, double min, double max, uint32 col, double d, double s, double t, double a);

	bool intersect(Vector *camera, Vector *ray, Vector *result, Vector *normal, double dt) override;

private:
	double min_bound;
	double max_bound;

};

/*
#define M 16
#define N 16

class SurfaceObject : public SceneObject {
public:
	float surfaceGrid[M][N][3];

	SurfaceObject();  // How to construct ?

	bool intersect(Vector *camera, Vector *ray, Vector *result); // ray intersect with surfaceGrid 
}
*/