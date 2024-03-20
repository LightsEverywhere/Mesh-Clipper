#include "pch.h"
#include "GUI.h"

CGUI::CGUI(void)
{
	coorRadius = 300;
	coorAmount = 2;
	coorPrecision = 10;

	interval = coorRadius / coorAmount;
	subInterval = coorRadius / (coorAmount * coorPrecision);
}

CGUI::~CGUI(void)
{
}

void CGUI::BindCamera(CProjection* camera)
{
	this->camera = camera;
}

void CGUI::DrawCoordinateGrid(CDC* pDC)
{
	CPen thinPen, boldPen, greenBoldPen, redBoldPen, * pOldPen;
	thinPen.CreatePen(PS_SOLID, 1, RGB(72, 72, 72));
	boldPen.CreatePen(PS_SOLID, 2, RGB(72, 72, 72));
	greenBoldPen.CreatePen(PS_SOLID, 2, RGB(100, 139, 37));
	redBoldPen.CreatePen(PS_SOLID, 2, RGB(148, 60, 72));

	CP3 worldP, screenP;
	// 绘制细分网格
	pOldPen = pDC->SelectObject(&thinPen);
	for (int x = -coorRadius; x <= coorRadius; x += subInterval)
	{
		worldP.x = x, worldP.y = 0, worldP.z = coorRadius;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->MoveTo(screenP.x, screenP.y);
		worldP.z = -coorRadius;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->LineTo(screenP.x, screenP.y);
	}
	for (int z = -coorRadius; z <= coorRadius; z += subInterval)
	{
		worldP.x = -coorRadius, worldP.y = 0, worldP.z = z;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->MoveTo(screenP.x, screenP.y);
		worldP.x = coorRadius;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->LineTo(screenP.x, screenP.y);
	}
	// 绘制主网格
	pDC->SelectObject(&boldPen);
	for (int x = -coorRadius; x <= coorRadius; x += interval)
	{
		worldP.x = x, worldP.y = 0, worldP.z = coorRadius;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->MoveTo(screenP.x, screenP.y);
		worldP.z = -coorRadius;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->LineTo(screenP.x, screenP.y);
	}
	for (int z = -coorRadius; z <= coorRadius; z += interval)
	{
		worldP.x = -coorRadius, worldP.y = 0, worldP.z = z;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->MoveTo(screenP.x, screenP.y);
		worldP.x = coorRadius;
		screenP = camera->PerspectiveProjection(worldP);
		pDC->LineTo(screenP.x, screenP.y);
	}

	// 标明坐标系X、Z轴
	pDC->SelectObject(&redBoldPen);// X轴
	worldP.x = -coorRadius, worldP.y = 0, worldP.z = 0;
	screenP = camera->PerspectiveProjection(worldP);
	pDC->MoveTo(screenP.x, screenP.y);
	worldP.x = coorRadius;
	screenP = camera->PerspectiveProjection(worldP);
	pDC->LineTo(screenP.x, screenP.y);
	pDC->SelectObject(&greenBoldPen);// Z轴
	worldP.x = 0, worldP.y = 0, worldP.z = -coorRadius;
	screenP = camera->PerspectiveProjection(worldP);
	pDC->MoveTo(screenP.x, screenP.y);
	worldP.z = coorRadius;
	screenP = camera->PerspectiveProjection(worldP);
	pDC->LineTo(screenP.x, screenP.y);

	pDC->SelectObject(pOldPen);
	thinPen.DeleteObject();
	boldPen.DeleteObject();
}

void CGUI::DrawCoordinateAxis(CDC* pDC)
{
	CPen newPen, * pOldPen;
	newPen.CreatePen(PS_SOLID, 2, RGB(59, 59, 59));
	pDC->SelectObject(&newPen);
}


