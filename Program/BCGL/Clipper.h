#pragma once
#include <vector>
#include "Triangle.h"
#include "Face.h"
#include "Projection.h"


struct CEdge
{
	CEdge(CP3 P0, CP3 P1) { P[0] = P0, P[1] = P1; }

	CP3 P[2];
};

class CClipper
{
public:
	CClipper(void);
	virtual ~CClipper(void);
	void Initialize(CP3* frustumPoint, CFace* frustumWall);// 传入frustum信息
	void SetMainCamera(CProjection* mainCamera);// 设置主相机
	void ReadMetaQueue(CTriangle* triangle,int triNum);// 读取元数据
	void Clipping();// 预处理队列
	void DrawWireFrame(CDC* pDC);

	std::vector<CTriangle> GetRenderQueue();

public:
	std::vector<CTriangle> metaQueue;// 元数据队列
	std::vector<CTriangle> renderQueue;// 渲染队列
	std::vector<CTriangle> clipQueue;// 裁剪队列

	CP3* frustumPoint;
	CFace* frustumWall;

	CProjection* camera;

};