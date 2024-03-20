#pragma once
#include"P3.h"
#include"Vector3.h"
#include"Face.h"

class CProjection
{
public:
	CProjection(void);
	virtual ~CProjection(void);
	void SetEye(double Phi, double Theta, double zNear = 500, double zFar = -500,double R=1000);// �����ӵ�
	CP3 GetEye(void);// ����ӵ�
	void InitialParameter(void);
	CP3 PerspectiveProjection(CP3 WorldPoint);
	void CalculateFrustum();// ����frustum����

	CP3 ViewTransform(CP3 WorldPoint);// View�任
	CP3 PerspTransform(CP3 ViewPoint);// ͸��Projection�任
	CP3 InverseViewTransform(CP3 ViewPoint);// ��View�任

	CP3* GetFrustumPoint();
	CFace* GetFrustumWall();

public:
	CP3 Eye;// �ӵ�
	double R, Phi, Theta, d;//�ӵ�������
	double k[8];// ͸��ͶӰ����

	double zNear, zFar;// ��Զ������
	double nearWidth;// ����������
	double aspectRatio;// �����

	CP3 frustumPoint[8];
	CFace frustumWall[6];

};

