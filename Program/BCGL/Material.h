#pragma once
#include"RGB.h"

class CMaterial
{
public:
	CMaterial(void);
	virtual ~CMaterial(void);
	void SetAmbientRef(CRGB ambientRef);// 设置环境反射率
	void SetDiffuseRef(CRGB diffuseRef);// 设置漫反射率
	void SetSpecularRef(CRGB specularRef);// 设置镜面反射率
	void SetSpecularExponent(double n);
	void SetAlbedo(CRGB albedo);// 设置反照率
	void SetMetallic(float metallic);// 设置金属度
	void SetRoughness(float roughness);// 设置粗糙度
	void SetAO(CRGB ao);// 设置环境光遮蔽
	void SetF0(CRGB F0);// 设置基础反射率
	void UpdateF0();
public:
	// Data for Blinn-Phong
	CRGB ambientRef;// 环境反射率
	CRGB diffuseRef;// 漫反射率
	CRGB specularRef;// 镜面反射率
	double specularExponent;// 高光指数
	// Data for PBR
	CRGB albedo;// 反照率
	float metallic;// 金属度
	float roughness;// 表面粗糙度
	CRGB ao;// 环境光遮蔽
	CRGB F0;// 基础反射率
};

