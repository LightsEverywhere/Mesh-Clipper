#pragma once
#include"Material.h"
#include"LightSource.h"
#include"Vector3.h"
#include"Skybox.h"

class CLighting
{
public:
	CLighting(void);
	virtual ~CLighting(void);
	void SetLightSource(CLightSource* lightSource);// 设置光源
	CRGB SimpleIlluminate(CP3 point, CP3 mainCamPos, CVector3 normal, const CMaterial* material);
	CRGB PBR(CP3 point, CMaterial* material, CVector3 normal);
	CRGB EnvPBR(CP3 point, CMaterial* material, CVector3 normal);
	CRGB EnvNewPBR(CP3 point, CMaterial* material, CVector3 normal);

	CRGB Radiance(CP3 point,CLightSource light);// 计算辐射率(仅考虑了距离衰减)
	CRGB CookTorranceBRDF(CP3 point, CP3 lightPos, CMaterial* material, CVector3 normal);
	CRGB EnvCTBRDF(CP3 point, CP3 lightPos, CMaterial* material, CVector3 normal);
	CRGB FresnelSchlick(float cosTheta, CRGB F0);// 计算Fresnel项
	CRGB FresnelSchlickRoughness(float cosTheta, CRGB F0, float roughness);// 计算Fresnel项(考虑粗糙度)
	float DistributionGGX(CVector3 normal, CVector3 H, float roughness);// 计算法线分布函数
	float GeometrySchlickGGX(float NdotV, float roughness);// 计算几何函数
	float GeometrySmith(CVector3 normal, CVector3 viewVector, CVector3 lightVector, float roughness);// 混合几何遮蔽与几何阴影
	float GeometrySchlickGGX_IBL(float NdotV, float roughness);// 计算几何函数
	float GeometrySmith_IBL(CVector3 normal, CVector3 viewVector, CVector3 lightVector, float roughness);// 混合几何遮蔽与几何阴影

public:
	CLightSource* lightSource;// 光源
	CP3* camPos;// 相机位置
	CSkybox* skybox;
};

