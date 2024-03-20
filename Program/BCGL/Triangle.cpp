#include "pch.h"
#include "Triangle.h"
#define CRGBtoRGB(c) RGB(c.red*255,c.green*255,c.blue*255)

CTriangle::CTriangle()
{
}

CTriangle::CTriangle(CP3 p0, CP3 p1, CP3 p2)
{
	P[0] = p0;
	P[1] = p1;
	P[2] = p2;
}

CTriangle::~CTriangle()
{
}