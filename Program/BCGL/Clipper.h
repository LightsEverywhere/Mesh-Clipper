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
	void Initialize(CP3* frustumPoint, CFace* frustumWall);// ����frustum��Ϣ
	void SetMainCamera(CProjection* mainCamera);// ���������
	void ReadMetaQueue(CTriangle* triangle,int triNum);// ��ȡԪ����
	void Clipping();// Ԥ�������
	void DrawWireFrame(CDC* pDC);

	std::vector<CTriangle> GetRenderQueue();

public:
	std::vector<CTriangle> metaQueue;// Ԫ���ݶ���
	std::vector<CTriangle> renderQueue;// ��Ⱦ����
	std::vector<CTriangle> clipQueue;// �ü�����

	CP3* frustumPoint;
	CFace* frustumWall;

	CProjection* camera;

};