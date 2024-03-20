#pragma once
#include"P3.h"

class CTriangle
{
public:
	CTriangle(void);
	CTriangle(CP3 p0, CP3 p1, CP3 p2);
	virtual ~CTriangle(void);
public:
	int vertexIndex[3];// 片元顶点索引
	int textureIndex[3];// 片元纹理点索引
	int normalIndex[3];// 片元法向索引

	CP3 P[3];
};

