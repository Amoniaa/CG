#pragma once

#include "Scene.h"
#include "Random.h"
#include "Vector.h"

class CCgRayTrace
{
public:
	CCgRayTrace();
	~CCgRayTrace();

	void runRayTrace(uint32 *bmpImag, int threadNum);

private:
	void renderScene(CCgScene *scene, Vector &camera, uint32 *pane, int32 width, int32 height);

};

