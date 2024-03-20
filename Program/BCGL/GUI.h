#pragma once
#include"P3.h"
#include"Projection.h"

class CGUI
{
public:
	CGUI(void);
	virtual ~CGUI(void);
	void BindCamera(CProjection* camera);// 绑定相机对象
	void DrawCoordinateGrid(CDC* pDC);// 绘制坐标系网格
	void DrawCoordinateAxis(CDC* pDC);// 绘制坐标系轴向
public:
	float coorRadius;// 网格轴向半径
	float coorAmount;// 沿轴某一方向的网格数量
	float coorPrecision;// 网格精度(细分网格数量)
	float interval, subInterval;//跨度

	CProjection* camera;// 相机对象
};

