#pragma once
#include"P3.h"

class CLightSource
{
public:
	CLightSource(void);
	virtual ~CLightSource(void);
	void SetLightPosition(CP3 lightPosition);// 设置光源位置
	void SetAmbientInten(CRGB ambientInten);// 设置环境光颜色
	void SetDiffuseInten(CRGB diffuseInten);// 设置漫反射光颜色
	void SetSpecularInten(CRGB specularInten);// 设置镜面光颜色
public:
	CRGB ambientInten;// 环境光颜色
	CRGB diffuseInten;// 漫反射光颜色
	CRGB specularInten;// 镜面光颜色

	CRGB lightColor;// 光的颜色
	CP3 lightPosition;// 光源位置
	double f;// 衰减系数
	BOOL lightOn;// 光源开启判定
};

