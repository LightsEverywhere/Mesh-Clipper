
// ProgramView.h: CProgramView 类的接口
//

#pragma once
#include"BCGL/GUI.h"
#include"BCGL/Model.h"
#include"BCGL/Transform3.h"
#include"BCGL/Skybox.h"
#include"BCGL/Clipper.h"

class CProgramView : public CView
{
protected: // 仅从序列化创建
	CProgramView() noexcept;
	DECLARE_DYNCREATE(CProgramView)

// 特性
public:
	CProgramDoc* GetDocument() const;

// 操作
public:
	void OffsetClientRect(CDC* pDC);
	void DoubleBuffer(CDC* pDC);// 双缓冲
	void DrawObject(CDC* pDC);
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CProgramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CRect rect;
	CProjection mainCamera;// 主相机
	CProjection testCamera;// 演示相机
	CGUI gui;
	CModel* model;// 外部导入模型对象
	enum DrawingMode {// 绘制模式
		pointCloud,
		wireFrame,
		phongShading,
		PBR,
		PBRxIBL
	} drawingMode;
	CTransform3 transform;// 变换对象
	CMaterial material;// (BRDF)名称待定
	CTexture* albedoTex;
	CTexture* normalTex;
	CTexture* roughnessTex;
	CTexture* metallicTex;
	CTexture* aoTex;

	CLighting scene;// 光照场景
	CSkybox* skybox;// 天空盒（立方体贴图）
	CTexture* envTexture;// 环境贴图
	CZBuffer* zbuffer;// z缓冲区
	BOOL isRButtonDown;
	CPoint lastMousePoint;

	CClipper clipper;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnImportObj();// obj文件导入
	afx_msg void OnLightCtrl_1();// 光源开关控制
	afx_msg void OnLightCtrl_2();
	afx_msg void OnLightCtrl_3();
	afx_msg void OnLightCtrl_4();
	afx_msg void OnAllLightsOn();
	afx_msg void OnAllLightsOff();
	afx_msg void OnPointCloudMode();
	afx_msg void OnWireFrameMode();
	afx_msg void OnPhongMode();
	afx_msg void OnPBRenderMode();
	afx_msg void OnPBRxIBLMode();
	afx_msg void OnLoadEnvironmentTex();
	afx_msg void OnMetallicZero();
	afx_msg void OnMetallicHalf();
	afx_msg void OnMetallicTotal();
	afx_msg void OnRoughnessZero();
	afx_msg void OnRoughnessHalf();
	afx_msg void OnRoughnessTotal();
	afx_msg void OnDefaultValue();
	afx_msg void OnImportAlbedo();
	afx_msg void OnImportNormal();
	afx_msg void OnImportMetallic();
	afx_msg void OnImportRoughness();
	afx_msg void OnImportAO();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ProgramView.cpp 中的调试版本
inline CProgramDoc* CProgramView::GetDocument() const
   { return reinterpret_cast<CProgramDoc*>(m_pDocument); }
#endif

