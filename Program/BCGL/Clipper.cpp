#include "pch.h"
#include "Clipper.h"

CClipper::CClipper(void)
{
}

CClipper::~CClipper(void)
{
}

void CClipper::Initialize(CP3* frustumPoint, CFace* frustumWall)
{
	this->frustumPoint = frustumPoint;
	this->frustumWall = frustumWall;

}

void CClipper::SetMainCamera(CProjection* mainCamera)
{
	this->camera = mainCamera;
}

void CClipper::ReadMetaQueue(CTriangle* triangle,int triNum)
{
	if (!metaQueue.empty())
		metaQueue.clear();
	for (int i = 0; i < triNum; i++)
	{
		metaQueue.push_back(triangle[i]);
	}
}

void CClipper::Clipping()
{
	std::vector<std::vector<CTriangle>> triQueBuf(2);// 交互存放每一步裁剪后的数据
	triQueBuf[0] = metaQueue;// 拷贝元数据队列
	int i = 0;
	for (int nWall = 0; nWall < 6; nWall++, i++)
	{
		CVector3 edgeVector01(frustumPoint[frustumWall[nWall].vertexIndex[1]] - frustumPoint[frustumWall[nWall].vertexIndex[0]]);
		CVector3 edgeVector02(frustumPoint[frustumWall[nWall].vertexIndex[2]] - frustumPoint[frustumWall[nWall].vertexIndex[0]]);
		CVector3 frustumNormal = CrossProduct(edgeVector01, edgeVector02).Normalized();

		std::vector<CTriangle>::iterator metaIt = triQueBuf[i%2].begin();

		for (; metaIt != triQueBuf[i % 2].end(); metaIt++)
		{
			std::vector<int> outPIndex;
			for (int nTri = 0; nTri < 3; nTri++)
			{
				CVector3 p(frustumPoint[frustumWall[nWall].vertexIndex[0]], metaIt->P[nTri]);

				if (DotProduct(p, frustumNormal) < 0)
				{
					outPIndex.push_back(nTri);
				}
			}
			if (outPIndex.size() == 0)
				triQueBuf[(i + 1) % 2].push_back(*metaIt);
			if (outPIndex.size() == 1)
			{
				CEdge prevEdge(metaIt->P[(outPIndex[0] + 2) % 3], metaIt->P[outPIndex[0]]);
				CEdge nextEdge(metaIt->P[outPIndex[0]], metaIt->P[(outPIndex[0] + 1) % 3]);
				CP3 intersectP[2];

				float t = 0;// 交点在直线中对应的参数t
				t = DotProduct(CVector3(prevEdge.P[0], frustumPoint[frustumWall[nWall].vertexIndex[0]]), frustumNormal) / (DotProduct(CVector3(prevEdge.P[0], prevEdge.P[1]), frustumNormal) + 0.001);
				intersectP[0] = prevEdge.P[0] + (prevEdge.P[1] - prevEdge.P[0]) * t;
				t = DotProduct(CVector3(nextEdge.P[0], frustumPoint[frustumWall[nWall].vertexIndex[0]]), frustumNormal) / (DotProduct(CVector3(nextEdge.P[0], nextEdge.P[1]), frustumNormal) + 0.001);
				intersectP[1] = nextEdge.P[0] + (nextEdge.P[1] - nextEdge.P[0]) * t;

				triQueBuf[(i + 1) % 2].push_back(CTriangle(intersectP[0], intersectP[1], metaIt->P[(outPIndex[0] + 1) % 3]));
				triQueBuf[(i + 1) % 2].push_back(CTriangle(metaIt->P[(outPIndex[0] + 2) % 3], intersectP[0], metaIt->P[(outPIndex[0] + 1) % 3]));

			}
			if (outPIndex.size() == 2)
			{
				if ((outPIndex[0] + outPIndex[1]) == 2)
					std::swap(outPIndex[0], outPIndex[1]);
				CEdge edge01(metaIt->P[(outPIndex[0] + 2) % 3], metaIt->P[outPIndex[0]]);
				CEdge edge02(metaIt->P[outPIndex[1]], metaIt->P[(outPIndex[1] + 1) % 3]);
				CP3 intersectP[2];

				float t = 0;
				float subDenominator = DotProduct(CVector3(edge01.P[0], frustumPoint[frustumWall[nWall].vertexIndex[0]]), frustumNormal);
				float denominator= DotProduct(CVector3(edge01.P[0], edge01.P[1]), frustumNormal);
				t = DotProduct(CVector3(edge01.P[0], frustumPoint[frustumWall[nWall].vertexIndex[0]]), frustumNormal) / (DotProduct(CVector3(edge01.P[0], edge01.P[1]), frustumNormal) + 0.001);
				intersectP[0] = edge01.P[0] + (edge01.P[1] - edge01.P[0]) * t;

				subDenominator = DotProduct(CVector3(edge02.P[0], frustumPoint[frustumWall[nWall].vertexIndex[0]]), frustumNormal);
				denominator = DotProduct(CVector3(edge02.P[0], edge02.P[1]), frustumNormal);
				t = DotProduct(CVector3(edge02.P[0], frustumPoint[frustumWall[nWall].vertexIndex[0]]), frustumNormal) / (DotProduct(CVector3(edge02.P[0], edge02.P[1]), frustumNormal) + 0.001);
				intersectP[1] = edge02.P[0] + (edge02.P[1] - edge02.P[0]) * t;

				triQueBuf[(i + 1) % 2].push_back(CTriangle(intersectP[0], intersectP[1], metaIt->P[(outPIndex[0] + 2) % 3]));

			}
		}

		triQueBuf[i % 2].clear();
		if (i == 5)
			renderQueue = triQueBuf[(i + 1) % 2];
	}
}

void CClipper::DrawWireFrame(CDC* pDC)
{
	CPen newPen, * pOldPen;
	newPen.CreatePen(BS_SOLID, 1, RGB(230, 230, 230));
	pDC->SelectObject(&newPen);
	for (auto it = renderQueue.begin(); it != renderQueue.end(); it++)
	{
		pDC->MoveTo(camera->PerspectiveProjection(it->P[0]).x, camera->PerspectiveProjection(it->P[0]).y);
		for (int nPoint = 1; nPoint < 3; nPoint++)
		{
			pDC->LineTo(camera->PerspectiveProjection(it->P[nPoint]).x, camera->PerspectiveProjection(it->P[nPoint]).y);
		}
		pDC->LineTo(camera->PerspectiveProjection(it->P[0]).x, camera->PerspectiveProjection(it->P[0]).y);
	}
	CPen boldPen;
	boldPen.CreatePen(BS_SOLID, 1, RGB(190, 190, 230));
	pDC->SelectObject(boldPen);

	for (int nWall = 0; nWall < 6; nWall++)
	{
		pDC->MoveTo(camera->PerspectiveProjection(frustumPoint[frustumWall[nWall].vertexIndex[0]]).x, camera->PerspectiveProjection(frustumPoint[frustumWall[nWall].vertexIndex[0]]).y);
		for (int nPoint = 1; nPoint < 4; nPoint++)
		{
			pDC->LineTo(camera->PerspectiveProjection(frustumPoint[frustumWall[nWall].vertexIndex[nPoint]]).x, camera->PerspectiveProjection(frustumPoint[frustumWall[nWall].vertexIndex[nPoint]]).y);
		}
		pDC->LineTo(camera->PerspectiveProjection(frustumPoint[frustumWall[nWall].vertexIndex[0]]).x, camera->PerspectiveProjection(frustumPoint[frustumWall[nWall].vertexIndex[0]]).y);
	}
}

std::vector<CTriangle> CClipper::GetRenderQueue()
{
	return renderQueue;
}

