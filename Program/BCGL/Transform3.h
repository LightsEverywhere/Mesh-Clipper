#pragma once
#include"P3.h"
#include"Vector3.h"

class CTransform3
{
public:
	CTransform3(void);
	virtual ~CTransform3(void);
	void SetMatrix(CP3* P, int ptNumber);// 三维顶点数组初始化
	void SetMatrix(CP3* P, CVector3* N, int ptNumber, int nNumber);
	void Identity(void);// 单位矩阵初始化
	void MultiplyMatrix(void);// 矩阵相乘

	void Translate(double tx, double ty, double tz);// 平移变换
	void Scale(double sx, double sy, double sz);// 比例变换
	void Scale(double sx, double sy, double sz, CP3 p);// 相对于任意点的比例变换

	void RotateX(double theta);
	void RotateY(double theta);
	void RotateZ(double theta);
	void RotateX(double theta, CP3 p);
	void RotateY(double theta, CP3 p);
	void RotateZ(double theta, CP3 p);

	void ReflectX(void);
	void ReflectY(void);
	void ReflectZ(void);

	void ReflectXOY(void);
	void ReflectXOZ(void);
	void ReflectYOZ(void);

	void ShearX(double sy, double sz);
	void ShearY(double sx, double sz);
	void ShearZ(double sx, double sy);

public:
	double	T[4][4];// 三维变换矩阵
	CP3* P;// 三维顶点数组名
	CVector3* N;// 点法向量数组名
	int	ptNumber;// 三维顶点个数
	int nNumber;// 点法向量个数
};

