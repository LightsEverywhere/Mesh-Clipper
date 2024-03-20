#include "pch.h"
#include "Lighting.h"

CLighting::CLighting(void)
{
	lightSource = new CLightSource[4];
	lightSource[0].lightPosition = CP3(1000, 1000, 1000);
	lightSource[0].lightColor = CRGB(70.0, 70.0, 70.0);

	lightSource[1].lightPosition = CP3(-1000, 800, 1000);
	lightSource[1].lightColor = CRGB(15.0, 15.0, 15.0);

	lightSource[2].lightPosition = CP3(500, 0, 1000);
	lightSource[2].lightColor = CRGB(15.0, 15.0, 15.0);

	lightSource[3].lightPosition = CP3(-700, -100, 1000);
	lightSource[3].lightColor = CRGB(15.0, 15.0, 15.0);
}

CLighting::~CLighting(void)
{
	if (lightSource != NULL)
	{
		delete[] lightSource;
		lightSource = NULL;
	}
}

void CLighting::SetLightSource(CLightSource* lightSource)
{
	this->lightSource = lightSource;
}

CRGB CLighting::SimpleIlluminate(CP3 point, CP3 mainCamPos,  CVector3 normal, const CMaterial* material)
{
	CRGB result = CRGB(0.0, 0.0, 0.0);

	CVector3 viewVector;// 视向量
	viewVector = CVector3(point, mainCamPos);
	viewVector.Normalize();

	for (int nLight = 0; nLight < 4; nLight++)
	{
		if (lightSource[nLight].lightOn)
		{
			CVector3 lightVector;// 光向量
			lightVector = CVector3(point, lightSource[nLight].lightPosition);
			double d = lightVector.Norm();// 光源到物体任意点的距离
			lightVector.Normalize();

			// 计算环境光部分
			CRGB La, ka, Ia;
			ka = material->ambientRef;
			Ia = lightSource[nLight].ambientInten * 0.25 / (4 * PI * d * d * 0.00000005);
			La = ka * Ia;

			// 计算漫反射光部分
			CRGB Ld, kd, Id;
			kd = material->diffuseRef;
			Id = lightSource[nLight].diffuseInten / (4 * PI * d * d * 0.00000005);
			Ld = kd * Id * max(DotProduct(normal, lightVector), 0);

			// 计算镜面反射光部分
			CRGB Ls, ks, Is;
			CVector3 halfwayVector;
			halfwayVector = (viewVector + lightVector).Normalized();
			ks = material->specularRef;
			Is = lightSource[nLight].specularInten / (4 * PI * d * d * 0.00000005);
			Ls = ks * Is * pow(max(DotProduct(normal, halfwayVector), 0), material->specularExponent);

			result += La + Ld + Ls;
		}
	}
	result.Normalize();
	return result;
}

CRGB CLighting::PBR(CP3 point, CMaterial* material, CVector3 normal)
{
	CRGB Lo = CRGB(0.0, 0.0, 0.0);

	CVector3 viewVector = CVector3(point, *camPos).Normalized();

	for (int nLight = 0; nLight < 4; nLight++)
	{
		if (lightSource[nLight].lightOn)
		{
			// 计算每个光源的radiance
			CVector3 lightVector = CVector3(point, lightSource[nLight].lightPosition);
			float distance = lightVector.Norm();// 光源到物体表面的距离
			lightVector.Normalize();
			float attenuation = 1.0 / (4 * PI * distance * distance * 0.00000005);
			CRGB radiance = lightSource[nLight].lightColor * attenuation;

			// 计算Cook-Torrance BRDF
			CVector3 H = (viewVector + lightVector).Normalized();
			float NDF = DistributionGGX(normal, H, material->roughness);
			float G = GeometrySmith(normal, viewVector, lightVector, material->roughness);
			CRGB F = FresnelSchlick(max(DotProduct(viewVector, H), 0.0), material->F0);

			CRGB kS = F;
			CRGB kD = 1.0 - kS;
			kD *= 1.0 - material->metallic;

			CRGB nominator = NDF * G * F;
			float denominator = 4.0 * max(DotProduct(viewVector, normal), 0.0) * max(DotProduct(lightVector, normal), 0.0) + 0.001;// 加0.001防止发生除零错误
			CRGB specular = nominator / denominator;
			CRGB diffuse = kD * material->albedo / PI;

			CRGB ctBRDF = diffuse + specular;

			float NdotL = max(DotProduct(normal, lightVector), 0.0);
			Lo += ctBRDF * radiance * NdotL;
		}
	}
	CRGB ambient = CRGB(0.03, 0.03, 0.03) * material->albedo;// 尚未加入环境光遮蔽
	Lo += ambient;

	return Lo;
}

CRGB CLighting::EnvPBR(CP3 point, CMaterial* material, CVector3 normal)
{
	CRGB Lo = CRGB(0.0, 0.0, 0.0);
	for (int nLight = 0; nLight < 4; nLight++)
	{
		if (lightSource[nLight].lightOn)
		{
			CVector3 lightVector = CVector3(point, lightSource[nLight].lightPosition).Normalized();// 计算多余，可以优化
			CRGB radiance = Radiance(point, lightSource[nLight]);
			CRGB ctBRDF = EnvCTBRDF(point, lightSource[nLight].lightPosition, material, normal);
			float NdotL = max(DotProduct(normal, lightVector), 0.0);
			Lo += ctBRDF * radiance * NdotL;
		}
	}
	return Lo;
}

CRGB CLighting::EnvNewPBR(CP3 point, CMaterial* material, CVector3 normal)
{
	CRGB Lo = CRGB(0.0, 0.0, 0.0);
	CVector3 viewVector = CVector3(point, *camPos).Normalized();
	CVector3 reflectVector = (2 * DotProduct(viewVector, normal) * normal - viewVector).Normalized();

	CRGB F = FresnelSchlickRoughness(DotProduct(viewVector, normal), material->F0, material->roughness);
	CRGB kS = F;
	CRGB kD = 1.0 - kS;
	kD *= 1.0 - material->metallic;

	//CRGB diffuse = kD * material->albedo;// 没有/PI，因为该计算写入了漫反射辐照度图中,(考虑把diffuse作为漫反射光，而不是材质漫反射参数）
	CRGB diffuse = kD * material->albedo / PI;

	CRGB pftColor = skybox->SamplePftCubeMap(reflectVector, material->roughness);
	CRGB envBRDF = skybox->SampleLut(DotProduct(normal, viewVector), material->roughness);
	CRGB specular = pftColor * (F * envBRDF.red + envBRDF.green);

	// Direct light
	for (int nLight = 0; nLight < 4; nLight++)
	{
		if (lightSource[nLight].lightOn)
		{
			// 计算每个光源的radiance
			CVector3 lightVector = CVector3(point, lightSource[nLight].lightPosition);
			float distance = lightVector.Norm();// 光源到物体表面的距离
			lightVector.Normalize();
			float attenuation = 1.0 / (4 * PI * distance * distance * 0.00000005);
			CRGB radiance = lightSource[nLight].lightColor * attenuation;

			// 计算Cook-Torrance BRDF
			CVector3 H = (viewVector + lightVector).Normalized();
			float NDF_d = DistributionGGX(normal, H, material->roughness);
			float G_d = GeometrySmith(normal, viewVector, lightVector, material->roughness);
			CRGB F_d = FresnelSchlick(max(DotProduct(viewVector, H), 0.0), material->F0);

			CRGB kS_d = F_d;
			CRGB kD_d = 1.0 - kS_d;
			kD_d *= 1.0 - material->metallic;

			CRGB nominator = NDF_d * G_d * F_d;
			float denominator = 4.0 * max(DotProduct(viewVector, normal), 0.0) * max(DotProduct(lightVector, normal), 0.0) + 0.001;// 加0.001防止发生除零错误
			CRGB specular_d = nominator / denominator;
			CRGB diffuse_d = kD_d * material->albedo / PI;

			CRGB ctBRDF = diffuse_d + specular_d;

			float NdotL = max(DotProduct(normal, lightVector), 0.0);
			Lo += ctBRDF * radiance * NdotL;
		}
	}

	CRGB ambient = diffuse * skybox->SampleIrrCubeMap(normal) + specular;
	//CRGB ambient = (diffuse * skybox->SampleIrrCubeMap(normal) + specular) * ao;// 如果有环境光遮蔽的话
	Lo += ambient;
	return Lo;
}

CRGB CLighting::Radiance(CP3 point, CLightSource light)
{
	float distance = CVector3(point, light.lightPosition).Norm();// 光源到物体表面的距离
	float attenuation = 1.0 / (4*PI*distance * distance* 0.00000005);
	CRGB radiance = light.lightColor * attenuation;
	return radiance;
}

CRGB CLighting::CookTorranceBRDF(CP3 point, CP3 lightPos,CMaterial* material, CVector3 normal)
{
	CVector3 viewVector = CVector3(point, *camPos).Normalized();
	CVector3 lightVector = CVector3(point, lightPos).Normalized();
	CVector3 H = (viewVector + lightVector).Normalized();

	float cosTheta = max(DotProduct(viewVector, H), 0.0);

	CRGB F = FresnelSchlick(cosTheta, material->F0);
	float NDF = DistributionGGX(normal, H, material->roughness);
	float G = GeometrySmith(normal, viewVector, lightVector, material->roughness);

	CRGB nominator = NDF * G * F;
	float denominator = 4 * DotProduct(normal, viewVector) * DotProduct(normal, lightVector) + 0.001;// 加0.001防止发生除零错误

	CRGB specular = nominator / denominator;// CookTorranceBRDF的镜面光项

	CRGB kS = F;
	CRGB kD = 1 - kS;

	kD *= 1.0 - material->metallic;

	CRGB diffuse = kD * material->albedo / PI;

	return (diffuse + specular);
}

CRGB CLighting::EnvCTBRDF(CP3 point, CP3 lightPos, CMaterial* material, CVector3 normal)
{
	CVector3 viewVector = CVector3(point, *camPos).Normalized();
	CVector3 lightVector = CVector3(point, lightPos).Normalized();
	CVector3 H = (viewVector + lightVector).Normalized();
	CVector3 reflectVector = (2 * DotProduct(viewVector, normal) * normal - viewVector).Normalized();
	

	float cosTheta = max(DotProduct(viewVector, H), 0.0);
	CRGB F = FresnelSchlickRoughness(cosTheta, material->F0, material->roughness);
	float NDF = DistributionGGX(normal, H, material->roughness);
	float G = GeometrySmith(normal, viewVector, lightVector, material->roughness);

	//CRGB nominator = NDF * G * F;
	//float denominator = 4 * DotProduct(normal, viewVector) * DotProduct(normal, lightVector) + 0.001;// 加0.001防止发生除零错误
	//CRGB specular = nominator / denominator;// CookTorranceBRDF的镜面光项


	//CRGB prefilteredColor = skybox->SamplePftCubeMap(reflectVector);
	CRGB prefilteredColor = skybox->SampleEnvCubeMap(reflectVector);
	//CRGB envBRDF = skybox->IntegrateBRDF(DotProduct(viewVector, normal), material->roughness);
	CRGB envBRDF = skybox->SampleLut(DotProduct(viewVector, normal), material->roughness);
	CRGB specular = prefilteredColor * (F * envBRDF.red + envBRDF.green);

	CRGB kS = F;
	CRGB kD = 1 - kS;

	kD *= 1.0 - material->metallic;
	CRGB irradiance = skybox->SampleIrrCubeMap(normal);
	CRGB diffuse = kD * irradiance * material->albedo / PI;

	return (diffuse + specular);
}

CRGB CLighting::FresnelSchlick(float cosTheta, CRGB F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

CRGB CLighting::FresnelSchlickRoughness(float cosTheta, CRGB F0, float roughness)
{
	double leastValue = min(F0.red, min(F0.green, F0.blue));
	return F0 + (max(1.0 - roughness, leastValue) - F0) * pow(1.0 - cosTheta, 5.0);
}

float CLighting::DistributionGGX(CVector3 normal, CVector3 H, float roughness)
{
	float a = roughness * roughness;// 将粗糙度转为粗糙度的平方，会让光照看起来更加自然
	float a2 = a * a;
	float NdotH = max(DotProduct(normal, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	
	float nom = a2;
	float denom = NdotH2 * (a2 - 1) + 1;
	denom = PI * denom * denom;

	return nom / denom;
}

float CLighting::GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;// 直接光照的k

	float nom = NdotV;
	float denom = NdotV * (1 - k) + k;

	return nom / denom;
}

float CLighting::GeometrySmith(CVector3 normal, CVector3 viewVector, CVector3 lightVector, float roughness)
{
	float NdotV = DotProduct(normal, viewVector);
	float NdotL = DotProduct(normal, lightVector);

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float CLighting::GeometrySchlickGGX_IBL(float NdotV, float roughness)
{
	float r = roughness;
	float k = (r * r) / 2.0;// IBL光照的k

	float nom = NdotV;
	float denom = NdotV * (1 - k) + k;

	return nom / denom;
}

float CLighting::GeometrySmith_IBL(CVector3 normal, CVector3 viewVector, CVector3 lightVector, float roughness)
{
	float NdotV = DotProduct(normal, viewVector);
	float NdotL = DotProduct(normal, lightVector);

	float ggx2 = GeometrySchlickGGX_IBL(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX_IBL(NdotL, roughness);

	return ggx1 * ggx2;
}
