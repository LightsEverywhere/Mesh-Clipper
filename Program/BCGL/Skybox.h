#pragma once
#include"P3.h"
#include"Face.h"
#include"Texture.h"
#include"Vector3.h"
#include"Projection.h"
#define PI 3.141592653

class CSkybox
{
public:
	CSkybox(void);
	virtual ~CSkybox(void);
	void SpheretoEnvCube(CTexture* targetTex);
	CRGB SampleEnvCubeMap(CVector3 vec3);// 从立方体环境贴图中获取颜色信息

	void SpheretoIrrCube(CTexture* targetTex);
	CRGB SampleIrrCubeMap(CVector3 vec3);// 从立方体辐照度图中获取颜色信息

	void ShowHammersley(CDC* pDC);// Hammersley序列可视化
	CP2 Hammersley(UINT i, UINT sampleCount);// Hammersley采样
	CVector3 ImportanceSampleGGX(CP2 Xi, CVector3 N, float roughness);// GGX重要性采样(该函数采样结果为一个半程向量H) *因为该函数根据NDF函数得到，NDF函数的输入为法向、半程向量、粗糙度
	void ShowGGX(CDC* pDC, CProjection* camera);// 显示重要性采样
	void SpheretoPrefilterMap(CTexture* targetTex);
	CRGB SamplePftCubeMap(CVector3 vec3, float roughness);
	CRGB IntegrateBRDF(float NdotV, float roughness);// 镜面BRDF积分
	void GenLookUpTexture();// 生成LUT
	void ShowLUT(CDC* pDC);// 显示LUT
	CRGB SampleLut(float NdotV, float roughness);// 从LUT中采样


	void ReadSubImg(int envType);// 从现成的立方体贴图获取颜色信息

	void DrawSamplePoint(CDC* pDC, CProjection* camera);

	void UnfoldSkybox(CDC* pDC);
	void UnfoldIrrBox(CDC* pDC);
	void UnfoldPftBox(CDC* pDC);
	void OutPutpftBox();

	float GeometrySchlickGGX_IBL(float NdotV, float roughness);// 计算几何函数
	float GeometrySmith_IBL(CVector3 normal, CVector3 viewVector, CVector3 lightVector, float roughness);// 混合几何遮蔽与几何阴影

public:
	CP3 vertex[8];
	CFace face[6];
	CTexture envSubTex[6];// 六个环境纹理面
	int envTexWidth, envTexHeight;// 各面立方体环境贴图分辨率 宽、高
	CTexture irrSubTex[6];// 六个辐照度图面
	int irrTexWidth, irrTexHeight;// 各面立方体辐照度图分辨率 宽、高
	//CTexture prefilterTex[6];// 六个预滤波贴图面
	CTexture pftSubTex[6][6];// ps处理后的与滤波贴图
	int pftTexWidth, pftTexHeight;// 各面立方体预滤波贴图分辨率 宽、高
	CTexture lutTex;// 2D查找纹理
	int lutTexWidth, lutTexHeight;// 2D查找纹理的分辨率 宽、高
};

