#include "pch.h"
#include "Vector3.h"

CVector3::CVector3()// 0ÏòÁ¿
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
}

CVector3::~CVector3()
{

}

CVector3::CVector3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 0.0;
}

CVector3::CVector3(const CP3& P)
{
	x = P.x;
	y = P.y;
	z = P.z;
	w = 0.0;
}

CVector3::CVector3(const CP3& P0, const CP3& P1)
{
	x = P1.x - P0.x;
	y = P1.y - P0.y;
	z = P1.z - P0.z;
	w = P1.w - P0.w;
}

CVector3::CVector3(const CP2& P)
{
	x = P.x;
	y = P.y;
	z = 0.0;
	w = 0.0;
}

CVector3::CVector3(const CP2& P0, const CP2& P1)
{
	x = P1.x - P0.x;
	y = P1.y - P0.y;
	z = 0.0;
	w = P1.w - P0.w;
}

double CVector3::Norm()
{
	return sqrt(x * x + y * y + z * z);
}

void CVector3::Normalize()
{
	double norm = Norm();
	if (norm < 1e-4)
	{
	}
	else
	{
		x = x / norm;
		y = y / norm;
		z = z / norm;
	}
}

CVector3 CVector3::Normalized()
{
	double norm = Norm();
	if (norm < 1e-4)
	{
		return CVector3(x, y, z);
	}
	else
	{
		return CVector3
		(
			x / norm,
			y / norm,
			z / norm
		);
	}
}

double DotProduct(const CVector3& V0, const CVector3& V1)
{
	double dotProduct;
	dotProduct = V0.x * V1.x + V0.y * V1.y + V0.z * V1.z;
	return dotProduct;
}

CVector3 CrossProduct(const CVector3& V0, const CVector3& V1)
{
	CVector3 crossProduct;
	crossProduct.x = V0.y * V1.z - V0.z * V1.y;
	crossProduct.y = V0.z * V1.x - V0.x * V1.z;
	crossProduct.z = V0.x * V1.y - V0.y * V1.x;
	return crossProduct;
}

CVector3 operator+(const CVector3& V0, const CVector3& V1)
{
	return CVector3
	(
		V0.x + V1.x,
		V0.y + V1.y,
		V0.z + V1.z
	);
}

CVector3 operator-(const CVector3& V0, const CVector3& V1)
{
	return CVector3
	(
		V0.x - V1.x,
		V0.y - V1.y,
		V0.z - V1.z
	);
}

CVector3 operator*(const CVector3& V, double scalar)
{
	return CVector3
	(
		V.x * scalar,
		V.y * scalar,
		V.z * scalar
	);
}

CVector3 operator*(double scalar, const CVector3& V)
{
	return CVector3
	(
		scalar * V.x,
		scalar * V.y,
		scalar * V.z
	);
}

CVector3 operator/(const CVector3& V, double scalar)
{
	return CVector3
	(
		V.x / scalar,
		V.y / scalar,
		V.z / scalar
	);
}

CVector3 operator+=(const CVector3& V0, const CVector3& V1)
{
	return CVector3
	(
		V0.x + V1.x,
		V0.y + V1.y,
		V0.z + V1.z
	);
}

CVector3 operator-=(const CVector3& V0, const CVector3& V1)
{
	return CVector3
	(
		V0.x - V1.x,
		V0.y - V1.y,
		V0.z - V1.z
	);
}

CVector3 operator*=(const CVector3& V, double scalar)
{
	return CVector3
	(
		V.x * scalar,
		V.y * scalar,
		V.z * scalar
	);
}

CVector3 operator/=(const CVector3& V, double scalar)
{
	return CVector3
	(
		V.x / scalar,
		V.y / scalar,
		V.z / scalar
	);
}
