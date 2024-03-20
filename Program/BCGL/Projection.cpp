#include "pch.h"
#include "Projection.h"
constexpr auto PI = 3.1415926;

CProjection::CProjection(void)
{
	R = 1000, d = 2000, Phi = 90, Theta = 0;//视点位于屏幕正前方		看向-z方向
	zNear = 500, zFar = -500;//近剪切面与远剪切面
	nearWidth = 640;
	aspectRatio = 16.0 / 9;
	InitialParameter();
}

CProjection::~CProjection(void)
{
}

void CProjection::SetEye(double Phi, double Theta, double zNear, double  zFar, double R)
{
	this->R = R, d = 2000;
	this->zNear = zNear, this->zFar = zFar;
	this->Phi = Phi;
	this->Theta = Theta;
	nearWidth = 320;
	aspectRatio = 16.0 / 9;
	InitialParameter();
}

CP3 CProjection::GetEye(void)
{
	return Eye;
}

void CProjection::InitialParameter(void)
{
	k[0] = sin(PI * Theta / 180);//Phi代表θ
	k[1] = sin(PI * Phi / 180);//Phi代表φ
	k[2] = cos(PI * Theta / 180);
	k[3] = cos(PI * Phi / 180);
	k[4] = k[1] * k[2];
	k[5] = k[0] * k[1];
	k[6] = k[2] * k[3];
	k[7] = k[0] * k[3];
	Eye = CP3(R * k[5], R * k[3], R * k[4]);//设置视点
}

CP3 CProjection::PerspectiveProjection(CP3 WorldPoint)
{
	CP3 ViewPoint;//观察坐标系三维点
	ViewPoint.x = k[2] * WorldPoint.x - k[0] * WorldPoint.z;
	ViewPoint.y = -k[7] * WorldPoint.x + k[1] * WorldPoint.y - k[6] * WorldPoint.z;
	ViewPoint.z = -k[5] * WorldPoint.x - k[3] * WorldPoint.y - k[4] * WorldPoint.z + R;
	ViewPoint.c = WorldPoint.c;
	CP3 ScreenPoint;//屏幕坐标系三维点
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.z = zFar * (1 - zNear / ViewPoint.z) / (zFar - zNear);
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}

void CProjection::CalculateFrustum()
{
	for (int nWall = 0; nWall < 6; nWall++)
	{
		frustumWall[nWall].vertexNumber = 4;
		frustumWall[nWall].InitializeQueue();
	}
	double nearHeight = nearWidth / aspectRatio;
	frustumPoint[0] = CP3( nearWidth / 2 * (R - zFar) / (R - zNear),  nearHeight / 2 * (R - zFar) / (R - zNear), zFar);
	frustumPoint[1] = CP3( nearWidth / 2 * (R - zFar) / (R - zNear), -nearHeight / 2 * (R - zFar) / (R - zNear), zFar);
	frustumPoint[2] = CP3( nearWidth / 2,  nearHeight / 2, zNear);
	frustumPoint[3] = CP3( nearWidth / 2, -nearHeight / 2, zNear);
	frustumPoint[4] = CP3(-nearWidth / 2 * (R - zFar) / (R - zNear),  nearHeight / 2 * (R - zFar) / (R - zNear), zFar);
	frustumPoint[5] = CP3(-nearWidth / 2 * (R - zFar) / (R - zNear), -nearHeight / 2 * (R - zFar) / (R - zNear), zFar);
	frustumPoint[6] = CP3(-nearWidth / 2,  nearHeight / 2, zNear);
	frustumPoint[7] = CP3(-nearWidth / 2, -nearHeight / 2, zNear);
	// 面表，环绕顺序已被改变，以保证frustum各面向内
	frustumWall[0].vertexIndex[3] = 0, frustumWall[0].vertexIndex[2] = 4, frustumWall[0].vertexIndex[1] = 6, frustumWall[0].vertexIndex[0] = 2;
	frustumWall[1].vertexIndex[3] = 3, frustumWall[1].vertexIndex[2] = 2, frustumWall[1].vertexIndex[1] = 6, frustumWall[1].vertexIndex[0] = 7;
	frustumWall[2].vertexIndex[3] = 7, frustumWall[2].vertexIndex[2] = 6, frustumWall[2].vertexIndex[1] = 4, frustumWall[2].vertexIndex[0] = 5;
	frustumWall[3].vertexIndex[3] = 5, frustumWall[3].vertexIndex[2] = 1, frustumWall[3].vertexIndex[1] = 3, frustumWall[3].vertexIndex[0] = 7;
	frustumWall[4].vertexIndex[3] = 1, frustumWall[4].vertexIndex[2] = 0, frustumWall[4].vertexIndex[1] = 2, frustumWall[4].vertexIndex[0] = 3;
	frustumWall[5].vertexIndex[3] = 5, frustumWall[5].vertexIndex[2] = 4, frustumWall[5].vertexIndex[1] = 0, frustumWall[5].vertexIndex[0] = 1;
}

CP3 CProjection::ViewTransform(CP3 WorldPoint)
{
	CP3 ViewPoint;//观察坐标系三维点
	ViewPoint.x = k[2] * WorldPoint.x - k[0] * WorldPoint.z;
	ViewPoint.y = -k[7] * WorldPoint.x + k[1] * WorldPoint.y - k[6] * WorldPoint.z;
	ViewPoint.z = -k[5] * WorldPoint.x - k[3] * WorldPoint.y - k[4] * WorldPoint.z + R;
	ViewPoint.c = WorldPoint.c;
	return ViewPoint;
}

CP3 CProjection::PerspTransform(CP3 ViewPoint)
{
	CP3 ScreenPoint;//屏幕坐标系三维点
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.z = zFar * (1 - zNear / ViewPoint.z) / (zFar - zNear);
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}

CP3 CProjection::InverseViewTransform(CP3 ViewPoint)
{
	CP3 WorldPoint;
	WorldPoint.x = k[2] * ViewPoint.x - k[7] * ViewPoint.y - k[5] * ViewPoint.z + R * k[5] * ViewPoint.w;
	WorldPoint.y = k[1] * ViewPoint.y - k[3] * ViewPoint.z + R * k[3] * ViewPoint.w;
	WorldPoint.z = -k[0] * ViewPoint.x - k[6] * ViewPoint.y - k[4] * ViewPoint.z + R * k[4] * ViewPoint.w;
	WorldPoint.c = ViewPoint.c;
	return WorldPoint;
}

CP3* CProjection::GetFrustumPoint()
{
	return frustumPoint;
}

CFace* CProjection::GetFrustumWall()
{
	return frustumWall;
}
