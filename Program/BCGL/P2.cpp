#include "pch.h"
#include "P2.h"

CP2::CP2(void)
{
	x = 0.0;
	y = 0.0;
	w = 1.0;
	c = CRGB(0.0, 0.0, 0.0);
}

CP2::~CP2(void)
{
}

CP2::CP2(double x, double y)
{
	this->x = x;
	this->y = y;
	this->w = 1.0;
	this->c = CRGB(0.0, 0.0, 0.0);
}

CP2::CP2(double x, double y, CRGB c)
{
	this->x = x;
	this->y = y;
	this->w = 1.0;
	this->c = c;
}

CP2 operator+(const CP2& p0, const CP2& p1)
{
	CP2 point;
	point.x = p0.x + p1.x;
	point.y = p0.y + p1.y;
	return point;
}

CP2 operator*(const CP2& p, double scalar)
{
	CP2 point;
	point.x = p.x * scalar;
	point.y = p.y * scalar;
	return point;
}

CP2 operator*(double scalar, const CP2& p)
{
	CP2 point;
	point.x = p.x * scalar;
	point.y = p.y * scalar;
	return point;
}

CP2 operator/(const CP2& p, double scalar)
{
	CP2 point;
	point.x = p.x / scalar;
	point.y = p.y / scalar;
	return point;
}

double GetDistance(const CP2& p0, const CP2& p1)
{
	return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}
