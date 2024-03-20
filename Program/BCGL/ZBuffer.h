#pragma once
#include"P3.h"

class CZBuffer
{
public:
	CZBuffer(void);
	virtual ~CZBuffer(void);
	void SetZBufferSize(int nWidth, int nHeight);
	void InitializeZBuffer();
public:
	double** depthBuffer;// 深度缓冲区
	CRGB** colorBuffer;// 颜色缓冲区
	int nWidth, nHeight;// 缓冲区宽度、高度
};

