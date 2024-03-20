#include "pch.h"
#include "Transform3.h"
constexpr double PI = 3.1415926;

CTransform3::CTransform3(void)
{
	P = NULL;
	N = NULL;
}

CTransform3::~CTransform3(void)
{
}

void CTransform3::SetMatrix(CP3* P, int ptNumber)
{
	this->P = P;
	this->ptNumber = ptNumber;
}

void CTransform3::SetMatrix(CP3* P, CVector3* N, int ptNumber, int nNumber)
{
	this->P = P;
	this->N = N;
	this->ptNumber = ptNumber;
	this->nNumber = nNumber;
}

void CTransform3::Identity(void)
{
	T[0][0] = 1.0; T[0][1] = 0.0; T[0][2] = 0.0; T[0][3] = 0.0;
	T[1][0] = 0.0; T[1][1] = 1.0; T[1][2] = 0.0; T[1][3] = 0.0;
	T[2][0] = 0.0; T[2][1] = 0.0; T[2][2] = 1.0; T[2][3] = 0.0;
	T[3][0] = 0.0; T[3][1] = 0.0; T[3][2] = 0.0; T[3][3] = 1.0;
}

void CTransform3::MultiplyMatrix(void)
{
	CP3* PTemp = new CP3[ptNumber];
	for (int i = 0; i < ptNumber; i++)
		PTemp[i] = P[i];
	for (int i = 0; i < ptNumber; i++)// 对顶点进行变换
	{
		P[i].x = T[0][0] * PTemp[i].x + T[0][1] * PTemp[i].y + T[0][2] * PTemp[i].z + T[0][3] * PTemp[i].w;
		P[i].y = T[1][0] * PTemp[i].x + T[1][1] * PTemp[i].y + T[1][2] * PTemp[i].z + T[1][3] * PTemp[i].w;
		P[i].z = T[2][0] * PTemp[i].x + T[2][1] * PTemp[i].y + T[2][2] * PTemp[i].z + T[2][3] * PTemp[i].w;
		P[i].w = T[3][0] * PTemp[i].x + T[3][1] * PTemp[i].y + T[3][2] * PTemp[i].z + T[3][3] * PTemp[i].w;
	}

	if (N != NULL)
	{
		CVector3* NTemp = new CVector3[nNumber];
		for (int j = 0; j < nNumber; j++)
			NTemp[j] = N[j];
		for (int j = 0; j < nNumber; j++)// 对点法向量进行变换
		{
			N[j].x = T[0][0] * NTemp[j].x + T[0][1] * NTemp[j].y + T[0][2] * NTemp[j].z + T[0][3] * NTemp[j].w;
			N[j].y = T[1][0] * NTemp[j].x + T[1][1] * NTemp[j].y + T[1][2] * NTemp[j].z + T[1][3] * NTemp[j].w;
			N[j].z = T[2][0] * NTemp[j].x + T[2][1] * NTemp[j].y + T[2][2] * NTemp[j].z + T[2][3] * NTemp[j].w;
			N[j].w = T[3][0] * NTemp[j].x + T[3][1] * NTemp[j].y + T[3][2] * NTemp[j].z + T[3][3] * NTemp[j].w;
		}
		delete[]NTemp;
	}
	delete[]PTemp;
}

void CTransform3::Translate(double tx, double ty, double tz)
{
	Identity();
	T[0][3] = tx;
	T[1][3] = ty;
	T[2][3] = tz;
	MultiplyMatrix();
}

void CTransform3::Scale(double sx, double sy, double sz)
{
	Identity();
	T[0][0] = sx;
	T[1][1] = sy;
	T[2][2] = sz;
	MultiplyMatrix();
}

void CTransform3::Scale(double sx, double sy, double sz, CP3 p)
{
	Translate(-p.x, -p.y, -p.z);
	Scale(sx, sy, sz);
	Translate(p.x, p.y, p.z);
}

void CTransform3::RotateX(double theta)
{
	Identity();
	double rad = theta * PI / 180;
	T[1][1] = cos(rad); T[1][2] = -sin(rad);
	T[2][1] = sin(rad); T[2][2] = cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateY(double theta)
{
	Identity();
	double rad = theta * PI / 180;
	T[0][0] = cos(rad); T[0][2] = sin(rad);
	T[2][0] = -sin(rad); T[2][2] = cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateZ(double theta)
{
	Identity();
	double rad = theta * PI / 180;
	T[0][0] = cos(rad); T[0][1] = sin(rad);
	T[1][0] = -sin(rad); T[1][1] = cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateX(double theta, CP3 p)
{
	Translate(-p.x, -p.y, -p.z);
	RotateX(theta);
	Translate(p.x, p.y, p.z);
}

void CTransform3::RotateY(double theta, CP3 p)
{
	Translate(-p.x, -p.y, -p.z);
	RotateY(theta);
	Translate(p.x, p.y, p.z);
}

void CTransform3::RotateZ(double theta, CP3 p)
{
	Translate(-p.x, -p.y, -p.z);
	RotateZ(theta);
	Translate(p.x, p.y, p.z);
}

void CTransform3::ReflectX(void)
{
	Identity();
	T[1][1] = -1;
	T[2][2] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectZ(void)
{
	Identity();
	T[0][0] = -1;
	T[1][1] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectY(void)
{
	Identity();
	T[0][0] = -1;
	T[2][2] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectXOY(void)
{
	Identity();
	T[0][0] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectXOZ(void)
{
	Identity();
	T[1][1] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectYOZ(void)
{
	Identity();
	T[2][2] = -1;
	MultiplyMatrix();
}

void CTransform3::ShearX(double sy,double sz)
{
	Identity();
	T[0][1] = sy;	
	T[0][2] = sz;
	MultiplyMatrix();
}

void CTransform3::ShearY(double sx, double sz)
{
	Identity();
	T[1][0] = sx;
	T[1][2] = sz;
	MultiplyMatrix();
}

void CTransform3::ShearZ(double sx, double sy)
{
	Identity();
	T[2][0] = sx;
	T[2][1] = sy;
	MultiplyMatrix();
}

