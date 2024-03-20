#include "pch.h"
#include "Material.h"

CMaterial::CMaterial(void)
{
	// Data for Blinn-Phong
	ambientRef = CRGB(0.03, 0.03, 0.03);
	diffuseRef = CRGB(0.3, 0.9, 0.4);
	specularRef = CRGB(1.0, 1.0, 1.0);
	specularExponent = 200;// 高光指数
	// Data for PBR
	//albedo = CRGB(0.3, 0.9, 0.4);
	albedo = CRGB(0.070, 0.103, 0.045);
	//albedo = CRGB(0.5, 0.4, 0.1);
	//albedo = CRGB(0.5, 0.0, 0.0);
	//albedo = CRGB(0.01, 0.01, 0.01);
	metallic = 0.95;
	roughness = 0.09;
	F0 = CRGB(0.04, 0.04, 0.04);// 基础反射率(Base Reflectivity)
	F0 = mix(F0, albedo, metallic);// 线性混叠函数，输出 (1-metallic)F0 + metallic*albedo , 用于调整金属的基础反射率
}

CMaterial::~CMaterial(void)
{
}

void CMaterial::SetAmbientRef(CRGB ambientRef)
{
	this->ambientRef = ambientRef;
}

void CMaterial::SetDiffuseRef(CRGB diffuseRef)
{
	this->diffuseRef = diffuseRef;
}

void CMaterial::SetSpecularRef(CRGB specularRef)
{
	this->specularRef = specularRef;
}

void CMaterial::SetSpecularExponent(double specularExponent)
{
	this->specularExponent = specularExponent;
}

void CMaterial::SetAlbedo(CRGB albedo)
{
	this->albedo = albedo;
}

void CMaterial::SetMetallic(float metallic)
{
	this->metallic = metallic;
}

void CMaterial::SetRoughness(float roughness)
{
	this->roughness = roughness;
}

void CMaterial::SetAO(CRGB ao)
{
	this->ao = ao;
}

void CMaterial::SetF0(CRGB F0)
{
	this->F0 = F0;
}

void CMaterial::UpdateF0()
{
	this->F0 = CRGB(0.04, 0.04, 0.04);
	this->F0 = mix(F0, albedo, metallic);
}

//CRGB CMaterial::mix(CRGB F0, CRGB albedo, float metallic)
//{
//	return (1 - metallic) * F0 + metallic * albedo;// 线性混叠
//}
