#pragma once
#include"P3.h"

class CTriangle
{
public:
	CTriangle(void);
	CTriangle(CP3 p0, CP3 p1, CP3 p2);
	virtual ~CTriangle(void);
public:
	int vertexIndex[3];// ƬԪ��������
	int textureIndex[3];// ƬԪ���������
	int normalIndex[3];// ƬԪ��������

	CP3 P[3];
};

