#include "pch.h"
#include "ZBuffer.h"

CZBuffer::CZBuffer(void)
{
}

CZBuffer::~CZBuffer(void)
{
	for (int i = 0; i < nWidth; i++)
	{
		if (depthBuffer[i] != NULL)
		{
			delete[] depthBuffer[i];
			depthBuffer[i] = NULL;
		}
	}
	if (depthBuffer != NULL)
	{
		delete[] depthBuffer;
		depthBuffer = NULL;
	}

}

void CZBuffer::SetZBufferSize(int nWidth, int nHeight)
{
	this->nWidth = nWidth;
	this->nHeight = nHeight;
}

void CZBuffer::InitializeZBuffer()
{
	depthBuffer = new double*[nWidth];
	for (int i = 0; i < nWidth; i++)
	{
		depthBuffer[i] = new double[nHeight];
	}
	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			depthBuffer[i][j] = 1000;// ×óÊÖÏµ
		}
	}
}
