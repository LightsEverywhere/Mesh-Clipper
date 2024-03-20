#pragma once
#include"P3.h"
#include"Vector3.h"
#include"Face.h"

class CProjection
{
public:
	CProjection(void);
	virtual ~CProjection(void);
	void SetEye(double Phi, double Theta, double zNear = 500, double zFar = -500,double R=1000);// 设置视点
	CP3 GetEye(void);// 获得视点
	void InitialParameter(void);
	CP3 PerspectiveProjection(CP3 WorldPoint);
	void CalculateFrustum();// 计算frustum数据

	CP3 ViewTransform(CP3 WorldPoint);// View变换
	CP3 PerspTransform(CP3 ViewPoint);// 透视Projection变换
	CP3 InverseViewTransform(CP3 ViewPoint);// 逆View变换

	CP3* GetFrustumPoint();
	CFace* GetFrustumWall();

public:
	CP3 Eye;// 视点
	double R, Phi, Theta, d;//视点球坐标
	double k[8];// 透视投影常数

	double zNear, zFar;// 近远剪切面
	double nearWidth;// 近剪切面宽度
	double aspectRatio;// 长宽比

	CP3 frustumPoint[8];
	CFace frustumWall[6];

};

