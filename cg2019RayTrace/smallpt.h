#pragma once

class CCgSmallpt
{
public:
	CCgSmallpt();
	~CCgSmallpt();

	int smallptRayTrace(int width, int height, unsigned int *sample, int samples, int threadNum);
};

