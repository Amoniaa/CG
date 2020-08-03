/*
TypeDefine.h
Define Some Useful Data Type in 3D Clipping and Projectting
*/
#if !defined TypeDef

// Use _PIXEL array --- DIB Image, keep Image.
typedef unsigned int _VPIXEL;

typedef struct
{
	int width, height;
	_VPIXEL* DibImage;
} _2DBitMap;

typedef struct Ipoint {
	int x, y;
} Ipoint_t, *Ipoint_p;

typedef struct Gpoint {
	float x, y, z;
} Gpoint_t, *Gpoint_p;

typedef struct poly {
	int      clipCount;
	int      polyCount;
	float    polyLight[10];
	bool     polyVisible;        // Polygon whther visible
	Gpoint_t polyObject[5];      // Original Object
	Gpoint_t transObject[5];     // Transform Original Object
	Gpoint_t clipObject[10];     // Clip Transform Object
	Gpoint_t projectObject[10];  // Project Clip Object
	Ipoint_t zBufferObject[10];  // Project Object Trans to Project Plane for Z-Buffer processing

	Gpoint_t viewDrawObject[5];        // View Draw Object
	Gpoint_t viewTransObject[5];       // View Transform Object
	Gpoint_t viewProjectObject[5];     // View Transform Project Object
} Poly_t, *Poly_p;

typedef struct object {
	int    polyCount;
	bool   objectVisible;
	Poly_t objectSpace[1200];
	float  transMatrix[4][3];
	Gpoint_t objCenter;
} Object_t, *Object_p;

#define TypeDef

#endif