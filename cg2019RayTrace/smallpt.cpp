#include "stdafx.h"
#include "smallpt.h"
#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

CCgSmallpt::CCgSmallpt()
{
}


CCgSmallpt::~CCgSmallpt()
{
}

#define M_PI       3.14159265358979323846   // pi

//随机函数，返回一个在[0,1]区间的浮点数
double drand48() {
	return (double)rand() / (double)RAND_MAX;
}

struct _Vector
{
	double _x, _y, _z;

	_Vector(double x = 0, double y = 0, double z = 0) :_x(x), _y(y), _z(z) {}

	_Vector operator+(const _Vector &b)const {
		return _Vector(_x + b._x, _y + b._y, _z + b._z);
	}
	_Vector operator-(const _Vector &b)const {
		return _Vector(_x - b._x, _y - b._y, _z - b._z);
	}
	_Vector operator*(double b) const {
		return _Vector(_x * b, _y * b, _z * b);
	}
	_Vector mult(const _Vector &b)const {
		return _Vector(_x * b._x, _y * b._y, _z * b._z);
	}
	// 向量标准化
	_Vector& normalize() {
		double t = sqrt(_x * _x + _y * _y + _z * _z);//向量的模
		if (t == 0) return *this;
		*this = this->operator*(1.0 / t);
		return *this;
	}
	//点乘
	double dot(const _Vector &b)const {
		return _x * b._x + _y * b._y + _z * b._z;
	}
	//叉乘
	_Vector cross(const _Vector &b)const {
		return _Vector(_y*b._z - _z*b._y, _z*b._x - _x*b._z, _x*b._y - _y*b._x);
	}
};

struct Ray {
	_Vector _origin, _direct;
	Ray(_Vector origin, _Vector direct) :_origin(origin), _direct(direct) {}
};

//材质类型:漫反射 镜面 折射
enum class Reflect { Diffuse, Specular, Refract };

struct Sphere {
	double _radius;
	_Vector _position, _emission, _color;
	Reflect _reflect;

	Sphere(double radius, _Vector position, _Vector emission, _Vector color, Reflect reflect) :
		_radius(radius), _position(position), _emission(emission), _color(color), _reflect(reflect) {}
	double intersect(const Ray &ray) const {
		_Vector op = _position - ray._origin;
		double epsilon = 1e-4;
		double half_b = op.dot(ray._direct);
		double delta_1_4 = half_b * half_b - op.dot(op) + _radius * _radius;
		if (delta_1_4 < 0)
			return 0;
		else {
			double sqrt_delta_1_4 = sqrt(delta_1_4);
			double temp = half_b - sqrt_delta_1_4;
			if (temp > epsilon) return temp;
			temp = half_b + sqrt_delta_1_4;
			if (temp > epsilon) return temp;
			return 0;
		}
	}
};

Sphere spheres[] = {//Scene: radius, position, emission, color, material
	Sphere(1e5, _Vector(1e5 + 1,40.8,81.6),  _Vector(),_Vector(.75,.25,.25),Reflect::Diffuse),//Left
	Sphere(1e5, _Vector(-1e5 + 99,40.8,81.6),_Vector(),_Vector(.25,.25,.75),Reflect::Diffuse),//Rght
	Sphere(1e5, _Vector(50,40.8, 1e5),       _Vector(),_Vector(.75,.75,.75),Reflect::Diffuse),//Back
	Sphere(1e5, _Vector(50,40.8,-1e5 + 170), _Vector(),_Vector(),           Reflect::Diffuse),//Frnt
	Sphere(1e5, _Vector(50, 1e5, 81.6),      _Vector(),_Vector(.75,.75,.75),Reflect::Diffuse),//Botm
	Sphere(1e5, _Vector(50,-1e5 + 81.6,81.6),_Vector(),_Vector(.75,.75,.75),Reflect::Diffuse),//Top
	Sphere(16.5,_Vector(27,16.5,47),         _Vector(),_Vector(1,1,1)*.999, Reflect::Specular),//Mirr
	Sphere(16.5,_Vector(73,16.5,78),         _Vector(),_Vector(1,1,1)*.999, Reflect::Refract),//Glas
	Sphere(600, _Vector(50,681.6 - .27,81.6),_Vector(12,12,12),  _Vector(), Reflect::Diffuse) //Lite
};

inline double clamp(double x) {
	return x < 0 ? 0 : (x > 1 ? 1 : x);
}
inline int gamma(double x) {
	return int(pow(clamp(x), 1 / 2.2) * 255 + 0.5);
}

inline bool intersect(const Ray &ray, double &distance, int &id) {
	int size = sizeof(spheres) / sizeof(Sphere);
	double d;
	double inf = distance = 1e20;
	for (int i = 0; i < size; i++) {
		d = spheres[i].intersect(ray);
		if (d != 0 && d < distance) {
			distance = d; id = i;
		}
	}
	return distance < inf;
}

_Vector radiance(const Ray &ray, int depth) {
	depth++;
	double distance;
	int id = 0;
	if (!intersect(ray, distance, id))	return _Vector();

	const Sphere &obj = spheres[id];
	_Vector hit_point = ray._origin + ray._direct * distance;
	_Vector normal = (hit_point - obj._position).normalize();
	_Vector normal_real = normal.dot(ray._direct) < 0 ? normal : normal * -1;
	_Vector f = obj._color;
	double p = fmax(fmax(f._x, f._y), f._z);
	if (depth > 5) {
		if (drand48() < p) f = f * (1 / p);
		else return obj._emission;
	}
	if (depth > 100) return obj._emission;

	switch (obj._reflect) {
	  case Reflect::Diffuse: {
		double r1 = 2 * M_PI * drand48();
		double r2 = drand48();
		double r2_sqrt = sqrt(r2);
		_Vector w = normal_real;
		_Vector u = (fabs(w._x) > 0.1 ? w.cross(_Vector(0, 1)) : w.cross(_Vector(1))).normalize();
		_Vector v = w.cross(u);
		_Vector direct = (u*cos(r1)*r2_sqrt + v*sin(r1)*r2_sqrt + w*sqrt(1 - r2));
		return obj._emission + f.mult(radiance(Ray(hit_point, direct), depth));
	  }	break;
	  case Reflect::Specular: {
		_Vector direct_refl = ray._direct - normal * 2 * normal.dot(ray._direct);
		return obj._emission + radiance(Ray(hit_point, direct_refl), depth);
	  }	break;
	  case Reflect::Refract: {  //反射光线	
		Ray ray_refl(hit_point, ray._direct - normal * 2 * normal.dot(ray._direct));
		bool into = normal.dot(normal_real) > 0;
		double nc = 1;                                   //真空
		double nt = 1.5;                                 //玻璃
		double nnt = into ? nc / nt : nt / nc;
		double ddn = ray._direct.dot(normal_real) * -1;  //入射角余弦

		double sin_2_t = (1 - ddn * ddn) * nnt * nnt;
		if (sin_2_t > 1.0)                               ////全内反射 sin^2(t)
			return obj._emission + f.mult(radiance(ray_refl, depth));
		double sin_t = sqrt(sin_2_t);
		double cos_t = sqrt(1 - sin_2_t);
		_Vector refr_direct = normal_real * (-1 * cos_t)
			+ (ray._direct + normal_real * ddn).normalize() * sin_t;
		Ray ray_refr(hit_point, refr_direct);

		double F0 = (nc - nt) * (nc - nt) / ((nc + nt) * (nc + nt));
		//double c = 1 - ddn;
		//double c = 1 + refr_direct.dot(normal_real);
		//选入射角与出射角中两者中角度较大的
		double c = 1 - (into ? ddn : refr_direct.dot(normal_real) * -1);
		double Fe = F0 + (1 - F0) * c * c * c * c * c;  //菲涅尔反射
		double Fr = 1 - Fe;                             //菲涅尔折射
		if (depth > 2) {
			double P = 0.25 + 0.5 * Fe;
			if (drand48() < P)
				return radiance(ray_refl, depth) * (Fe / P);
			else
				return radiance(ray_refr, depth) * (Fr / (1 - P));
		}
		else {
			return radiance(ray_refl, depth) * Fe
				 + radiance(ray_refr, depth) * Fr;
		}
	  }	break;
	default:
		break;
	}
}

static double viewMatrix[4][3];

void makeViewMatrix();
void viewTransPoint(_Vector& vPoint, double transMatrix[4][3]);
void vline(int x1, int y1, int x2, int y2, int width, int height, unsigned int *bmpImage);

int CCgSmallpt::smallptRayTrace(int width, int height, unsigned int *bmpImage, int samples, int threadNum)
{
	_Vector *_content = new _Vector[width * height];
	int a = threadNum;
#pragma omp parallel for num_threads(threadNum)
	for (int y = 0; y < height; y++) {              // 遍历行

		// Shared varibles 
		Ray camera(_Vector(50, 52, 295.6), _Vector(0, -0.042612, -1).normalize());
		_Vector cx(0.5135 * width / height);
		_Vector cy = cx.cross(camera._direct).normalize() * 0.5135;
		_Vector *content = _content;

		for (int x = 0; x < width; x++) {           //遍历列
			for (int sy = 0; sy < 2; sy++) {
				for (int sx = 0; sx < 2; sx++) {
					_Vector rad;
					for (int i = 0; i < samples; i++) {
						double r1 = 2 * drand48();
						double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * drand48();
						double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						_Vector sample_direct = cx*(((dx + 0.5 + sx) / 2 + x) / width - 0.5)
							                  + cy*(((dy + 0.5 + sy) / 2 + y) / height - 0.5) + camera._direct;
						rad = rad + radiance(Ray(camera._origin + sample_direct*140, sample_direct.normalize()), 0)*(1.0 / samples);
					}
					int i = (height - y - 1) *width + x;
					content[i] = content[i] + _Vector(clamp(rad._x), clamp(rad._y), clamp(rad._z)) * 0.25;
				}
			}
		}
	}
	// Save the image after tone mapping and gamma correction
	for (int i = 0; i < width*height; i++) {
		unsigned int red   = gamma(_content[i]._x);
		unsigned int green = gamma(_content[i]._y);
		unsigned int blue  = gamma(_content[i]._z);

		bmpImage[i] = (0xFF << 24) | (red << 16) | (green << 8) | blue;
	}


	_Vector ballC1(27, 16.5, 47);
	_Vector ballC2(73, 16.5, 78);
	_Vector ballC3(50, 81.7, 81.6);
	_Vector eyePos(50, 52, 295.6);      // eye position

	int x[3] = {0, 0, 0};
	int y[3] = {0, 0, 0};

	makeViewMatrix();
	viewTransPoint(eyePos, viewMatrix);
	viewTransPoint(ballC1, viewMatrix);
	viewTransPoint(ballC2, viewMatrix);
	viewTransPoint(ballC3, viewMatrix);

	// be sure that view angle is 0.5135 ?
	double Lw = (width * 0.5)  / (140 * sin(0.5135) * 0.5);
	double Lh = (height * 0.5) / (140 * sin(0.5135) * 0.5);

	double u = (0.0 - eyePos._z) / (ballC1._z - eyePos._z);
	x[0] = (int)(((ballC1._x - eyePos._x) * u + eyePos._x) * Lw + 0.5) + width / 2;  // ballC1
	y[0] = height / 2 - (int)(((ballC1._y - eyePos._y) * u + eyePos._y) * Lh + 0.5);

	u = (0.0 - eyePos._z) / (ballC2._z - eyePos._z);
	x[1] = (int)(((ballC2._x - eyePos._x) * u + eyePos._x) * Lw + 0.5) + width / 2;  // ballC2
	y[1] = height / 2 - (int)(((ballC2._y - eyePos._y) * u + eyePos._y) * Lh + 0.5);

	u = (0.0 - eyePos._z) / (ballC3._z - eyePos._z);
	x[2] = (int)(((ballC3._x - eyePos._x) * u + eyePos._x) * Lw + 0.5) + width / 2;  // ballC3
	y[2] = height / 2 - (int)(((ballC3._y - eyePos._y) * u + eyePos._y) * Lh + 0.5);

	vline(x[0], y[0], x[1], y[1], width, height, bmpImage);
	vline(x[1], y[1], x[2], y[2], width, height, bmpImage);

	return 0;
}

void _SetPixel(int x, int y, int red, int green, int blue, 
	           int width, int height, unsigned int *bmpImage)
{
	BOOL overSide = FALSE;
	int pos = x + y*width;
	int bmpSize = width*height;

	if (pos < 0) { pos = 0; overSide = TRUE; }
	if (pos >= bmpSize) { pos = bmpSize - 1; overSide = TRUE; }

	if (overSide) bmpImage[pos] = (255 << 16) + (255 << 8);
	else          bmpImage[pos] = (red << 16) + (green << 8) + blue;

}

void  vline(int x1, int y1, int x2, int y2, int width, int height, unsigned int *bmpImage)
{
	BOOL interChange = FALSE;
	int e, x, y, dx, dy, xSign, ySign;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dy > dx) {
		interChange = TRUE;
		e = dx;  dx = dy;  dy = e;
	}

	x = x1;
	y = y1;
	e = 2 * dy - dx;
	xSign = (x2 > x1) ? 1 : -1;
	ySign = (y2 > y1) ? 1 : -1;

	for (int i = 0; i < dx; i++) {

		_SetPixel(x, y, 255, 255, 255, width, height, bmpImage);
		if (e > 0) {
			e = e - 2 * dx;
			if (interChange) x += xSign;
			else             y += ySign;
		}

		e = e + 2 * dy;
		if (interChange) y += ySign;
		else             x += xSign;
	}
}

void rotateX3Dmatrix(double S, double C, double transMatrix[4][3]);
void rotateY3Dmatrix(double S, double C, double transMatrix[4][3]);
void rotateZ3Dmatrix(double S, double C, double transMatrix[4][3]);
void translate3DMatrix(_Vector vPoint, double transMatrix[4][3]);

void makeViewMatrix()
{
	_Vector eyePos(50, 52, 295.6);      // eye position
	_Vector eyeDir(0, -0.042612, -1);   // view direction

	eyeDir = eyeDir.normalize();
	double vDistance = 140*cos(0.5135);            // be sure that view angle is 0.5135 ?
	_Vector oPoint = eyePos + eyeDir * vDistance;  // original point on project plane.

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) viewMatrix[i][j] = 1.0;
			else        viewMatrix[i][j] = 0.0;
		}
	}

	translate3DMatrix(oPoint*(-1.0f), viewMatrix);

	double L = sqrt(eyeDir._y*eyeDir._y+ eyeDir._z*eyeDir._z);
	rotateX3Dmatrix(-eyeDir._y/L, -eyeDir._z/L, viewMatrix);
	rotateY3Dmatrix(eyeDir._x, L, viewMatrix);

	_Vector cx(1, 0, 0);
	_Vector cy = cx.cross(eyeDir).normalize();
    
	//  acoording cy calclulate upMatrix
	double upMatrix[4][3];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) upMatrix[i][j] = 1.0;
			else        upMatrix[i][j] = 0.0;
		}
	}

	rotateX3Dmatrix(-eyeDir._y / L, -eyeDir._z / L, upMatrix);
	rotateY3Dmatrix(eyeDir._x, L, upMatrix);

	viewTransPoint(cy, upMatrix);
	cy.normalize();

	rotateZ3Dmatrix(cy._x, cy._y, viewMatrix);
}

void viewTransPoint(_Vector& vPoint, double transMatrix[4][3])
{
	double tempx,tempy,tempz;

	tempx = vPoint._x * transMatrix[0][0] + vPoint._y * transMatrix[1][0] +
		    vPoint._z * transMatrix[2][0] + transMatrix[3][0];
	tempy = vPoint._x * transMatrix[0][1] + vPoint._y * transMatrix[1][1] +
		    vPoint._z * transMatrix[2][1] + transMatrix[3][1];
	tempz = vPoint._x * transMatrix[0][2] + vPoint._y * transMatrix[1][2] +
		    vPoint._z * transMatrix[2][2] + transMatrix[3][2];

	vPoint._x = tempx;
	vPoint._y = tempy;
	vPoint._z = tempz;
}

void translate3DMatrix(_Vector vPoint, double transMatrix[4][3])
{

	transMatrix[3][0] += vPoint._x;
	transMatrix[3][1] += vPoint._y;
	transMatrix[3][2] += vPoint._z;
}

void rotateX3Dmatrix(double S, double C, double transMatrix[4][3])
{

	for (int i = 0; i < 4; i++) {
		double temp;
		temp = transMatrix[i][1] * C - transMatrix[i][2] * S;
		transMatrix[i][2] = transMatrix[i][1] * S + transMatrix[i][2] * C;
		transMatrix[i][1] = temp;
	}
}

void rotateY3Dmatrix(double S, double C, double transMatrix[4][3])
{

	for (int i = 0; i < 4; i++) {
		double temp;
		temp = transMatrix[i][0] * C + transMatrix[i][2] * S;
		transMatrix[i][2] = -transMatrix[i][0] * S + transMatrix[i][2] * C;
		transMatrix[i][0] = temp;
	}
}

void rotateZ3Dmatrix(double S, double C, double transMatrix[4][3])
{

	for (int i = 0; i < 4; i++) {
		double temp;
		temp = transMatrix[i][0] * C - transMatrix[i][1] * S;
		transMatrix[i][1] = transMatrix[i][0] * S + transMatrix[i][1] * C;
		transMatrix[i][0] = temp;
	}
}
