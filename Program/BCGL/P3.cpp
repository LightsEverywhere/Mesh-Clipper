#include "pch.h"
#include "P3.h"

CP3::CP3(void)
{
	z = 0.0;
}

CP3::~CP3(void)
{
}

CP3::CP3(double x, double y, double z) :CP2(x, y)
{
	this->z = z;
}

CP3::CP3(double x, double y, double z, CRGB c) :CP2(x,y,c)
{
	this->z = z;
}

CP3 operator+(const CP3& p0, const CP3& p1)
{
	CP3 p;
	p.x = p1.x + p0.x;
	p.y = p1.y + p0.y;
	p.z = p1.z + p0.z;
	return p;
}

CP3 operator-(const CP3& p0, const CP3& p1)
{
	CP3 p;
	p.x = p0.x - p1.x;
	p.y = p0.y - p1.y;
	p.z = p0.z - p1.z;
	return p;
}

CP3 operator*(const CP3& p, double scalar)
{
	CP3 point;
	point.x = p.x * scalar;
	point.y = p.y * scalar;
	point.z = p.z * scalar;
	return point;
}

CP3 operator*(double scalar, const CP3& p)
{
	CP3 point;
	point.x = p.x * scalar;
	point.y = p.y * scalar;
	point.z = p.z * scalar;
	return point;
}

CP3 operator/(const CP3& p, double scalar)
{
	CP3 point;
	point.x = p.x / scalar;
	point.y = p.y / scalar;
	point.z = p.z / scalar;
	return point;
}

CP3 operator+=(CP3& p0, CP3& p1)
{
	p0.x += p1.x;
	p0.y += p1.y;
	p0.z += p1.z;
	return p0;
}

CP3 operator-=(CP3& p0, CP3& p1)
{
	p0.x -= p1.x;
	p0.y -= p1.y;
	p0.z -= p1.z;
	return p0;
}

CP3 operator*=(CP3& p, double scalar)
{
	p.x *= scalar;
	p.y *= scalar;
	p.z *= scalar;
	return p;
}

CP3 operator/=(CP3& p, double scalar)
{
	p.x /= scalar;
	p.y /= scalar;
	p.z /= scalar;
	return p;
}

double GetDistance(const CP3& p0, const CP3& p1)
{
	return sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y) + (p1.z - p0.z)*(p1.z - p0.z));
}
