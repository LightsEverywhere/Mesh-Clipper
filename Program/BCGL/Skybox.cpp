#include "pch.h"
#include "Skybox.h"
#define CRGBtoRGB(c) RGB(c.red*255,c.green*255,c.blue*255)

CSkybox::CSkybox(void)
{
	//// 点表
	//vertex[0] = CP3(0.5, 0.5, -0.5);
	//vertex[1] = CP3(0.5, -0.5, -0.5);
	//vertex[2] = CP3(0.5, 0.5, 0.5);
	//vertex[3] = CP3(0.5, -0.5, 0.5);
	//vertex[4] = CP3(-0.5, 0.5, -0.5);
	//vertex[5] = CP3(-0.5, -0.5, -0.5);
	//vertex[6] = CP3(-0.5, 0.5, 0.5);
	//vertex[7] = CP3(-0.5, -0.5, 0.5);
	//// 面表
	//for (int nFace = 0; nFace < 6; nFace++)
	//{
	//	face[nFace].vertexNumber = 4;
	//	face[nFace].InitializeQueue();
	//}
	//face[0].vertexIndex[0] = 1, face[0].vertexIndex[1] = 5, face[0].vertexIndex[2] = 7, face[0].vertexIndex[3] = 3;// Right
	//face[1].vertexIndex[0] = 4, face[1].vertexIndex[1] = 3, face[1].vertexIndex[2] = 7, face[1].vertexIndex[3] = 8;// Left
	//face[2].vertexIndex[0] = 8, face[2].vertexIndex[1] = 7, face[2].vertexIndex[2] = 5, face[2].vertexIndex[3] = 6;// Top
	//face[3].vertexIndex[0] = 6, face[3].vertexIndex[1] = 2, face[3].vertexIndex[2] = 4, face[3].vertexIndex[3] = 8;// Bottom
	//face[4].vertexIndex[0] = 2, face[4].vertexIndex[1] = 1, face[4].vertexIndex[2] = 3, face[4].vertexIndex[3] = 4;// Back
	//face[5].vertexIndex[0] = 6, face[5].vertexIndex[1] = 5, face[5].vertexIndex[2] = 1, face[5].vertexIndex[3] = 2;// Front
	// 立方体贴图分辨率
	envTexWidth = 512, envTexHeight = 512;
	irrTexWidth = 64, irrTexHeight = 64;
	pftTexWidth = 128, pftTexHeight = 128;
	lutTexWidth = 512, lutTexHeight = 512;
}

CSkybox::~CSkybox(void)
{
}

void CSkybox::SpheretoEnvCube(CTexture* targetTex)
{
	for (int nFace = 0; nFace < 6; nFace++)
	{
		envSubTex[nFace].CreateImgData(envTexWidth, envTexHeight, targetTex->nChannel);
		for (int realV = 0; realV < envTexHeight; realV++)
		{
			for (int realU = 0; realU < envTexWidth; realU++)
			{
				double u, v;
				u = realU * 1.0 / envTexWidth, v = realV * 1.0 / envTexHeight;
				CVector3 vec3;
				// 根据不同的面确定不同的方向向量
				if (nFace == 0) vec3 = CVector3(0.5, v - 0.5, u - 0.5).Normalized();
				else if (nFace == 1) vec3 = CVector3(-0.5, v - 0.5, 0.5 - u).Normalized();
				else if (nFace == 2) vec3 = CVector3(u - 0.5, 0.5, v - 0.5).Normalized();
				else if (nFace == 3) vec3 = CVector3(u - 0.5, -0.5, 0.5 - v).Normalized();
				else if (nFace == 4) vec3 = CVector3(0.5 - u, v - 0.5, 0.5).Normalized();
				else if (nFace == 5) vec3 = CVector3(u - 0.5, v - 0.5, -0.5).Normalized();

				CRGB color = targetTex->SampleSphericalMap(vec3);
				int position = (realU + (envTexHeight - 1 - realV) * envTexWidth) * envSubTex[nFace].nChannel;// 此处envTexHeight - 1 - realV将图像v方向取反，方便使用统一采样函数

				envSubTex[nFace].imgData[position] = color.red;
				envSubTex[nFace].imgData[position + 1] = color.green;
				envSubTex[nFace].imgData[position + 2] = color.blue;
			}
		}
	}
}

CRGB CSkybox::SampleEnvCubeMap(CVector3 vec3)
{
	double u, v;// 将要返回的采样点坐标
	float fabsX, fabsY, fabsZ;
	fabsX = fabs(vec3.x), fabsY = fabs(vec3.y), fabsZ = fabs(vec3.z);
	if (fabsX >= fabsY && fabsX >= fabsZ)// x分量为主要贡献，选取Right、Left面
	{
		if (vec3.x > 0)// Z对u，Y对v
		{
			u = (vec3.z / fabsX + 1) / 2;
			v = (vec3.y / fabsX + 1) / 2;
			return envSubTex[0].SampleTexture(CT2(u, v));
		}
		else// -Z对u，Y对v
		{
			u = (-vec3.z / fabsX + 1) / 2;
			v = (vec3.y / fabsX + 1) / 2;
			return envSubTex[1].SampleTexture(CT2(u, v));
		}
	}
	else if (fabsY >= fabsX && fabsY >= fabsZ)
	{
		if (vec3.y > 0)// X对u，Z对v
		{
			u = (vec3.x / fabsY + 1) / 2;
			v = (vec3.z / fabsY + 1) / 2;
			return envSubTex[2].SampleTexture(CT2(u, v));
		}
		else// X对u，-Z对v
		{
			u = (vec3.x / fabsY + 1) / 2;
			v = (-vec3.z / fabsY + 1) / 2;
			return envSubTex[3].SampleTexture(CT2(u, v));
		}
	}
	else if (fabsZ >= fabsX && fabsZ >= fabsY)
	{
		if (vec3.z > 0)// -X对u，Y对v
		{
			u = (-vec3.x / fabsZ + 1) / 2;
			v = (vec3.y / fabsZ + 1) / 2;
			return envSubTex[4].SampleTexture(CT2(u, v));
		}
		else// X对u，Y对v
		{
			u = (vec3.x / fabsZ + 1) / 2;
			v = (vec3.y / fabsZ + 1) / 2;
			return envSubTex[5].SampleTexture(CT2(u, v));
		}
	}
}

void CSkybox::SpheretoIrrCube(CTexture* targetTex)
{
	for (int nFace = 0; nFace < 6; nFace++)
	{
		irrSubTex[nFace].CreateImgData(irrTexWidth, irrTexHeight, targetTex->nChannel);
		for (int realV = 0; realV < irrTexHeight; realV++)
		{
			for (int realU = 0; realU < irrTexWidth; realU++)
			{
				double u, v;
				u = realU * 1.0 / irrTexWidth, v = realV * 1.0 / irrTexHeight;
				CVector3 vec3;// 采样输出方向
				// 根据不同的面确定不同的方向向量
				if (nFace == 0) vec3 = CVector3(0.5, v - 0.5, u - 0.5).Normalized();
				else if (nFace == 1) vec3 = CVector3(-0.5, v - 0.5, 0.5 - u).Normalized();
				else if (nFace == 2) vec3 = CVector3(u - 0.5, 0.5, v - 0.5).Normalized();
				else if (nFace == 3) vec3 = CVector3(u - 0.5, -0.5, 0.5 - v).Normalized();
				else if (nFace == 4) vec3 = CVector3(0.5 - u, v - 0.5, 0.5).Normalized();
				else if (nFace == 5) vec3 = CVector3(u - 0.5, v - 0.5, -0.5).Normalized();

				CRGB irradiance = CRGB(0.0, 0.0, 0.0);

				CVector3 up = CVector3(0.0, 1.0, 0.0);
				CVector3 normal = vec3;// N
				CVector3 right = CrossProduct(up, normal);// T
				if (right.Norm() <= 1e-4)//若right为零向量
				{
					right.x = 1.0, right.y = 1.0, right.z = -(normal.x + normal.y) / (normal.z + 0.001);
					right.Normalize();
				}
				CVector3 front = CrossProduct(normal, right).Normalized();// B

				//float sampleDelta = 0.025;// 球面采样步长
				float sampleDelta = 0.025;
				float nrSamples = 0.0;// 总采样数量
				for (float phi = 0; phi < 2 * PI; phi += sampleDelta)
				{
					for (float theta = 0; theta < PI / 2; theta += sampleDelta)
					{
						//spherical to cartesian (in tangent space)
						CVector3 tanSampleVec = CVector3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));// phi从T方向开始，绕N正向顺时针移动
						// tangent space to world space
						CVector3 sampleVec = CVector3(
							DotProduct(CVector3(right.x, front.x, normal.x), tanSampleVec),
							DotProduct(CVector3(right.y, front.y, normal.y), tanSampleVec),
							DotProduct(CVector3(right.z, front.z, normal.z), tanSampleVec)).Normalized();

						irradiance += targetTex->SampleSphericalMap(sampleVec) * cos(theta) * sin(theta);
						nrSamples++;
					}
				}

				irradiance = irradiance / nrSamples;
				int position = (realU + (irrTexHeight - 1 - realV) * irrTexWidth) * irrSubTex[nFace].nChannel;// 此处irrTexWidth - 1 - realV将图像v方向取反，方便使用统一采样函数

				irrSubTex[nFace].imgData[position] = irradiance.red;
				irrSubTex[nFace].imgData[position + 1] = irradiance.green;
				irrSubTex[nFace].imgData[position + 2] = irradiance.blue;
			}
		}
	}
}

CRGB CSkybox::SampleIrrCubeMap(CVector3 vec3)
{
	double u, v;// 将要返回的采样点坐标
	float fabsX, fabsY, fabsZ;
	fabsX = fabs(vec3.x), fabsY = fabs(vec3.y), fabsZ = fabs(vec3.z);
	if (fabsX >= fabsY && fabsX >= fabsZ)// x分量为主要贡献，选取Right、Left面
	{
		if (vec3.x > 0)// Z对u，Y对v
		{
			u = (vec3.z / fabsX + 1) / 2;
			v = (vec3.y / fabsX + 1) / 2;
			return irrSubTex[0].SampleTexture(CT2(u, v));
		}
		else// -Z对u，Y对v
		{
			u = (-vec3.z / fabsX + 1) / 2;
			v = (vec3.y / fabsX + 1) / 2;
			return irrSubTex[1].SampleTexture(CT2(u, v));
		}
	}
	else if (fabsY >= fabsX && fabsY >= fabsZ)
	{
		if (vec3.y > 0)// X对u，Z对v
		{
			u = (vec3.x / fabsY + 1) / 2;
			v = (vec3.z / fabsY + 1) / 2;
			return irrSubTex[2].SampleTexture(CT2(u, v));
		}
		else// X对u，-Z对v
		{
			u = (vec3.x / fabsY + 1) / 2;
			v = (-vec3.z / fabsY + 1) / 2;
			return irrSubTex[3].SampleTexture(CT2(u, v));
		}
	}
	else if (fabsZ >= fabsX && fabsZ >= fabsY)
	{
		if (vec3.z > 0)// -X对u，Y对v
		{
			u = (-vec3.x / fabsZ + 1) / 2;
			v = (vec3.y / fabsZ + 1) / 2;
			return irrSubTex[4].SampleTexture(CT2(u, v));
		}
		else// X对u，Y对v
		{
			u = (vec3.x / fabsZ + 1) / 2;
			v = (vec3.y / fabsZ + 1) / 2;
			return irrSubTex[5].SampleTexture(CT2(u, v));
		}
	}
}

void CSkybox::ShowHammersley(CDC* pDC)
{
	float radicalInverse;
	for (UINT n = 0; n < 300; n++)
	{
		UINT i = n;
		i = (i << 16u) | (i >> 16u);
		i = ((i & 0x55555555u) << 1u) | ((i & 0xAAAAAAAAu) >> 1u);
		i = ((i & 0x33333333u) << 2u) | ((i & 0xCCCCCCCCu) >> 2u);
		i = ((i & 0x0F0F0F0Fu) << 4u) | ((i & 0xF0F0F0F0u) >> 4u);
		i = ((i & 0x00FF00FFu) << 8u) | ((i & 0xFF00FF00u) >> 8u);
		radicalInverse =  float(i) * 2.3283064365386963e-10; // / 0x100000000
		CP3 samplePoint = CP3(200.0*float(n) / float(300), 200*radicalInverse, 0);
		pDC->SetPixelV(samplePoint.x, samplePoint.y,RGB(255,255,255));
	}
}

CP2 CSkybox::Hammersley(UINT i, UINT sampleCount)
{
	float radicalInverse;
	UINT num = i;
	num = (num << 16u) | (num >> 16u);
	num = ((num & 0x55555555u) << 1u) | ((num & 0xAAAAAAAAu) >> 1u);
	num = ((num & 0x33333333u) << 2u) | ((num & 0xCCCCCCCCu) >> 2u);
	num = ((num & 0x0F0F0F0Fu) << 4u) | ((num & 0xF0F0F0F0u) >> 4u);
	num = ((num & 0x00FF00FFu) << 8u) | ((num & 0xFF00FF00u) >> 8u);
	radicalInverse = float(num) * 2.3283064365386963e-10; // / 0x100000000

	return CP2(float(i) / float(sampleCount), radicalInverse);
}

CVector3 CSkybox::ImportanceSampleGGX(CP2 Xi, CVector3 N, float roughness)
{
	float a = roughness * roughness;

	// 根据逆置CDF获取采样向量
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + Xi.y * (a * a - 1.0)));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	// spherical to cartesian (in tangent space)
	CVector3 tanSampleVec = CVector3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

	// tangent space to world space
	CVector3 up = CVector3(0.0, 1.0, 0.0);
	CVector3 normal = N;// N
	CVector3 right = CrossProduct(up, normal);// T
	if (right.Norm() <= 1e-4)//若right为零向量
	{
		right.x = 1.0, right.y = 1.0, right.z = -(normal.x + normal.y) / (normal.z + 0.001);
		right.Normalize();
	}
	CVector3 front = CrossProduct(normal, right).Normalized();// B

	CVector3 sampleVec = CVector3(
		DotProduct(CVector3(right.x, front.x, normal.x), tanSampleVec),
		DotProduct(CVector3(right.y, front.y, normal.y), tanSampleVec),
		DotProduct(CVector3(right.z, front.z, normal.z), tanSampleVec)).Normalized();

	return sampleVec;
}

void CSkybox::ShowGGX(CDC* pDC, CProjection* camera)
{
	CVector3 up = CVector3(0.0, 1.0, 0.0);
	CVector3 normal = CVector3(0.0, 1.0, 0.0).Normalized();// N
	CVector3 right = CrossProduct(up, normal);// T
	if (right.Norm() <= 1e-4)//若right为零向量
	{
		right.x = 1.0, right.y = 1.0, right.z = -(normal.x + normal.y) / (normal.z + 0.001);
		right.Normalize();
	}
	CVector3 front = CrossProduct(normal, right).Normalized();// B

	CVector3 vec3 = normal;
	for (int i = 0; i < 4096; i++)
	{
		CP2 Xi = Hammersley(i, 300);
		CVector3 H = ImportanceSampleGGX(Xi, vec3, 0.4);// 粗糙度梯度需要调整！！！！！！！！！！！！
		CVector3 L = (2.0 * DotProduct(vec3, H) * H - vec3).Normalized();// 获取光向量

		//CP3 samplePoint = CP3(L.x * 50, L.y * 50, L.z * 50);
		CP3 samplePoint = CP3(H.x * 50, H.y * 50, H.z * 50);
		CP3 screenP = camera->PerspectiveProjection(samplePoint);
		CRGB color = CRGB(1.0, 1.0, 1.0);
		pDC->SetPixelV(screenP.x, screenP.y, CRGBtoRGB(color));
	}
}

void CSkybox::SpheretoPrefilterMap(CTexture* targetTex)
{
	for (int pftLevel = 3; pftLevel < 6; pftLevel++)
	{
		float roughness = 0;
		if (pftLevel == 1) roughness = 0.2;
		if (pftLevel == 2) roughness = 0.4;
		if (pftLevel == 3) roughness = 0.6;
		if (pftLevel == 4) roughness = 0.8;
		if (pftLevel == 5) roughness = 1.0;

		for (int nFace = 0; nFace < 6; nFace++)
		{
			pftSubTex[pftLevel][nFace].CreateImgData(pftTexWidth, pftTexHeight, targetTex->nChannel);
			for (int realV = 0; realV < pftTexHeight; realV++)
			{
				for (int realU = 0; realU < pftTexWidth; realU++)
				{
					double u, v;
					u = realU * 1.0 / pftTexWidth, v = realV * 1.0 / pftTexHeight;
					CVector3 vec3;// 采样输出方向
					// 根据不同的面确定不同的方向向量
					if (nFace == 0) vec3 = CVector3(0.5, v - 0.5, u - 0.5).Normalized();
					else if (nFace == 1) vec3 = CVector3(-0.5, v - 0.5, 0.5 - u).Normalized();
					else if (nFace == 2) vec3 = CVector3(u - 0.5, 0.5, v - 0.5).Normalized();
					else if (nFace == 3) vec3 = CVector3(u - 0.5, -0.5, 0.5 - v).Normalized();
					else if (nFace == 4) vec3 = CVector3(0.5 - u, v - 0.5, 0.5).Normalized();
					else if (nFace == 5) vec3 = CVector3(u - 0.5, v - 0.5, -0.5).Normalized();

					const UINT sampleCount = 4096u;
					float totalWeight = 0.0;
					CRGB prefilteredColor = CRGB(0.0, 0.0, 0.0);

					for (UINT i = 0u; i < sampleCount; i++)
					{
						CP2 Xi = Hammersley(i, sampleCount);
						CVector3 H = ImportanceSampleGGX(Xi, vec3, roughness);// 粗糙度梯度需要调整！！！！！！！！！！！！
						CVector3 L = (2.0 * DotProduct(vec3, H) * H - vec3).Normalized();// 获取光向量

						float NdotL = max(DotProduct(vec3, L), 0.0);
						if (NdotL > 0.0)
						{
							prefilteredColor += targetTex->SampleSphericalMap(L);
							totalWeight += NdotL;
						}
					}
					prefilteredColor = prefilteredColor / totalWeight;

					int position = (realU + (pftTexHeight - 1 - realV) * pftTexWidth) * pftSubTex[pftLevel][nFace].nChannel;// 此处irrTexWidth - 1 - realV将图像v方向取反，方便使用统一采样函数

					pftSubTex[pftLevel][nFace].imgData[position] = prefilteredColor.red;
					pftSubTex[pftLevel][nFace].imgData[position + 1] = prefilteredColor.green;
					pftSubTex[pftLevel][nFace].imgData[position + 2] = prefilteredColor.blue;
				}
			}
		}
	}
}

CRGB CSkybox::SamplePftCubeMap(CVector3 vec3, float roughness)
{
	int pftLevel;// 采样pft贴图的级别
	if (roughness <= 0.1) pftLevel = 0;
	if (roughness > 0.1 && roughness <= 0.3) pftLevel = 1;
	if (roughness > 0.3 && roughness <= 0.5) pftLevel = 2;
	if (roughness > 0.5 && roughness <= 0.7) pftLevel = 3;
	if (roughness > 0.7 && roughness <= 0.9) pftLevel = 4;
	if (roughness > 0.9) pftLevel = 5;
	double u, v;// 将要返回的采样点坐标
	float fabsX, fabsY, fabsZ;
	fabsX = fabs(vec3.x), fabsY = fabs(vec3.y), fabsZ = fabs(vec3.z);
	if (fabsX >= fabsY && fabsX >= fabsZ)// x分量为主要贡献，选取Right、Left面
	{
		if (vec3.x > 0)// Z对u，Y对v
		{
			u = (vec3.z / fabsX + 1) / 2;
			v = (vec3.y / fabsX + 1) / 2;
			return pftSubTex[pftLevel][0].SampleTexture(CT2(u, v));
		}
		else// -Z对u，Y对v
		{
			u = (-vec3.z / fabsX + 1) / 2;
			v = (vec3.y / fabsX + 1) / 2;
			return pftSubTex[pftLevel][1].SampleTexture(CT2(u, v));
		}
	}
	else if (fabsY >= fabsX && fabsY >= fabsZ)
	{
		if (vec3.y > 0)// X对u，Z对v
		{
			u = (vec3.x / fabsY + 1) / 2;
			v = (vec3.z / fabsY + 1) / 2;
			return pftSubTex[pftLevel][2].SampleTexture(CT2(u, v));
		}
		else// X对u，-Z对v
		{
			u = (vec3.x / fabsY + 1) / 2;
			v = (-vec3.z / fabsY + 1) / 2;
			return pftSubTex[pftLevel][3].SampleTexture(CT2(u, v));
		}
	}
	else if (fabsZ >= fabsX && fabsZ >= fabsY)
	{
		if (vec3.z > 0)// -X对u，Y对v
		{
			u = (-vec3.x / fabsZ + 1) / 2;
			v = (vec3.y / fabsZ + 1) / 2;
			return pftSubTex[pftLevel][4].SampleTexture(CT2(u, v));
		}
		else// X对u，Y对v
		{
			u = (vec3.x / fabsZ + 1) / 2;
			v = (vec3.y / fabsZ + 1) / 2;
			return pftSubTex[pftLevel][5].SampleTexture(CT2(u, v));
		}
	}
}

CRGB CSkybox::IntegrateBRDF(float NdotV, float roughness)
{
	CVector3 V;
	V.x = sqrt(1.0 - NdotV * NdotV);
	V.y = 0.0;
	V.z = NdotV;

	float scalePart = 0.0, biasPart = 0.0;

	CVector3 normal = CVector3(0.0, 0.0, 1.0);

	const UINT sampleCount = 1024u;
	for (UINT i = 0u; i < sampleCount; i++)
	{
		CP2 Xi = Hammersley(i, sampleCount);
		CVector3 H = ImportanceSampleGGX(Xi, normal, roughness);// 粗糙度梯度需要调整！！！！！！！！！！！！
		CVector3 L = (2.0 * DotProduct(normal, H) * H - normal).Normalized();// 获取光向量

		float NdotL = max(DotProduct(normal, L), 0.0);
		float NdotH = max(DotProduct(normal, H), 0.0);
		float VdotH = max(DotProduct(V, H), 0.0);

		if (NdotL > 0.0)
		{
			// LearnOpenGL版本
			float G = GeometrySmith_IBL(normal, V, L, roughness);
			float G_vis = (G * VdotH) / (NdotH * NdotV);

			float Fc = pow((1 - VdotH), 5.0);

			scalePart += (1.0 - Fc) * G_vis;
			biasPart += Fc * G_vis;
		}
	}
	scalePart /= float(sampleCount);
	biasPart /= float(sampleCount);

	return CRGB(scalePart, biasPart, 0.0);
}

void CSkybox::GenLookUpTexture()
{
	lutTex.CreateImgData(lutTexWidth, lutTexHeight, 3);

	for (int realV = 0; realV < lutTexHeight; realV++)
	{
		for (int realU = 0; realU < lutTexWidth; realU++)
		{
			float NdotV = float(realU) / lutTexWidth;
			float roughness = float(realV) / lutTexHeight;
			CRGB brdf = IntegrateBRDF(NdotV, roughness);

			int position = (realU + (lutTexHeight - 1 - realV) * lutTexWidth) * lutTex.nChannel;// 此处envTexWidth - 1 - realV将图像v方向取反，方便使用统一采样函数

			lutTex.imgData[position] = brdf.red;
			lutTex.imgData[position + 1] = brdf.green;
			lutTex.imgData[position + 2] = brdf.blue;
		}
	}
}

void CSkybox::ShowLUT(CDC* pDC)
{
	lutTex.ShowTexture(pDC);
}

CRGB CSkybox::SampleLut(float NdotV, float roughness)
{
	return lutTex.SampleTexture(CT2(NdotV, roughness));
}

void CSkybox::ReadSubImg(int envType)
{
	if (envType == 0)
	{
		// 环境贴图
		envSubTex[0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_0.hdr");
		envSubTex[0].ReadImagefData();
		envSubTex[1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_1.hdr");
		envSubTex[1].ReadImagefData();
		envSubTex[2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_2.hdr");
		envSubTex[2].ReadImagefData();
		envSubTex[3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_3.hdr");
		envSubTex[3].ReadImagefData();
		envSubTex[4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_4.hdr");
		envSubTex[4].ReadImagefData();
		envSubTex[5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_5.hdr");
		envSubTex[5].ReadImagefData();
		// 辐照度图
		irrSubTex[0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\irrSub\\Irradiance_0.hdr");
		irrSubTex[0].ReadImagefData();
		irrSubTex[1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\irrSub\\Irradiance_1.hdr");
		irrSubTex[1].ReadImagefData();
		irrSubTex[2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\irrSub\\Irradiance_2.hdr");
		irrSubTex[2].ReadImagefData();
		irrSubTex[3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\irrSub\\Irradiance_3.hdr");
		irrSubTex[3].ReadImagefData();
		irrSubTex[4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\irrSub\\Irradiance_4.hdr");
		irrSubTex[4].ReadImagefData();
		irrSubTex[5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\irrSub\\Irradiance_5.hdr");
		irrSubTex[5].ReadImagefData();
		// 预滤波贴图
		pftSubTex[0][0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_0.hdr");
		pftSubTex[0][0].ReadImagefData();
		pftSubTex[0][1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_1.hdr");
		pftSubTex[0][1].ReadImagefData();
		pftSubTex[0][2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_2.hdr");
		pftSubTex[0][2].ReadImagefData();
		pftSubTex[0][3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_3.hdr");
		pftSubTex[0][3].ReadImagefData();
		pftSubTex[0][4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_4.hdr");
		pftSubTex[0][4].ReadImagefData();
		pftSubTex[0][5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\envSub\\environment_5.hdr");
		pftSubTex[0][5].ReadImagefData();

		pftSubTex[1][0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.2_0.hdr");
		pftSubTex[1][0].ReadImagefData();
		pftSubTex[1][1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.2_1.hdr");
		pftSubTex[1][1].ReadImagefData();
		pftSubTex[1][2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.2_2.hdr");
		pftSubTex[1][2].ReadImagefData();
		pftSubTex[1][3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.2_3.hdr");
		pftSubTex[1][3].ReadImagefData();
		pftSubTex[1][4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.2_4.hdr");
		pftSubTex[1][4].ReadImagefData();
		pftSubTex[1][5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.2_5.hdr");
		pftSubTex[1][5].ReadImagefData();

		pftSubTex[2][0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.4_0.hdr");
		pftSubTex[2][0].ReadImagefData();
		pftSubTex[2][1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.4_1.hdr");
		pftSubTex[2][1].ReadImagefData();
		pftSubTex[2][2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.4_2.hdr");
		pftSubTex[2][2].ReadImagefData();
		pftSubTex[2][3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.4_3.hdr");
		pftSubTex[2][3].ReadImagefData();
		pftSubTex[2][4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.4_4.hdr");
		pftSubTex[2][4].ReadImagefData();
		pftSubTex[2][5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.4_5.hdr");
		pftSubTex[2][5].ReadImagefData();

		pftSubTex[3][0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.6_0.hdr");
		pftSubTex[3][0].ReadImagefData();
		pftSubTex[3][1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.6_1.hdr");
		pftSubTex[3][1].ReadImagefData();
		pftSubTex[3][2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.6_2.hdr");
		pftSubTex[3][2].ReadImagefData();
		pftSubTex[3][3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.6_3.hdr");
		pftSubTex[3][3].ReadImagefData();
		pftSubTex[3][4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.6_4.hdr");
		pftSubTex[3][4].ReadImagefData();
		pftSubTex[3][5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.6_5.hdr");
		pftSubTex[3][5].ReadImagefData();

		pftSubTex[4][0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.8_0.hdr");
		pftSubTex[4][0].ReadImagefData();
		pftSubTex[4][1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.8_1.hdr");
		pftSubTex[4][1].ReadImagefData();
		pftSubTex[4][2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.8_2.hdr");
		pftSubTex[4][2].ReadImagefData();
		pftSubTex[4][3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.8_3.hdr");
		pftSubTex[4][3].ReadImagefData();
		pftSubTex[4][4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.8_4.hdr");
		pftSubTex[4][4].ReadImagefData();
		pftSubTex[4][5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r0.8_5.hdr");
		pftSubTex[4][5].ReadImagefData();

		pftSubTex[5][0].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r1.0_0.hdr");
		pftSubTex[5][0].ReadImagefData();
		pftSubTex[5][1].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r1.0_1.hdr");
		pftSubTex[5][1].ReadImagefData();
		pftSubTex[5][2].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r1.0_2.hdr");
		pftSubTex[5][2].ReadImagefData();
		pftSubTex[5][3].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r1.0_3.hdr");
		pftSubTex[5][3].ReadImagefData();
		pftSubTex[5][4].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r1.0_4.hdr");
		pftSubTex[5][4].ReadImagefData();
		pftSubTex[5][5].SetImagePath("..\\PictureSource\\HDRi\\blue_photo_studio_4k\\PS_pftSub\\Prefilter_r1.0_5.hdr");
		pftSubTex[5][5].ReadImagefData();
	}
	if (envType == 1)
	{
		// 环境贴图
		envSubTex[0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_0.hdr");
		envSubTex[0].ReadImagefData();
		envSubTex[1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_1.hdr");
		envSubTex[1].ReadImagefData();
		envSubTex[2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_2.hdr");
		envSubTex[2].ReadImagefData();
		envSubTex[3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_3.hdr");
		envSubTex[3].ReadImagefData();
		envSubTex[4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_4.hdr");
		envSubTex[4].ReadImagefData();
		envSubTex[5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_5.hdr");
		envSubTex[5].ReadImagefData();
		// 辐照度图
		irrSubTex[0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\irrSub\\Irradiance_0.hdr");
		irrSubTex[0].ReadImagefData();
		irrSubTex[1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\irrSub\\Irradiance_1.hdr");
		irrSubTex[1].ReadImagefData();
		irrSubTex[2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\irrSub\\Irradiance_2.hdr");
		irrSubTex[2].ReadImagefData();
		irrSubTex[3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\irrSub\\Irradiance_3.hdr");
		irrSubTex[3].ReadImagefData();
		irrSubTex[4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\irrSub\\Irradiance_4.hdr");
		irrSubTex[4].ReadImagefData();
		irrSubTex[5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\irrSub\\Irradiance_5.hdr");
		irrSubTex[5].ReadImagefData();
		// 预滤波贴图
		pftSubTex[0][0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_0.hdr");
		pftSubTex[0][0].ReadImagefData();
		pftSubTex[0][1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_1.hdr");
		pftSubTex[0][1].ReadImagefData();
		pftSubTex[0][2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_2.hdr");
		pftSubTex[0][2].ReadImagefData();
		pftSubTex[0][3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_3.hdr");
		pftSubTex[0][3].ReadImagefData();
		pftSubTex[0][4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_4.hdr");
		pftSubTex[0][4].ReadImagefData();
		pftSubTex[0][5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\envSub\\environment_5.hdr");
		pftSubTex[0][5].ReadImagefData();

		pftSubTex[1][0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.2_0.hdr");
		pftSubTex[1][0].ReadImagefData();
		pftSubTex[1][1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.2_1.hdr");
		pftSubTex[1][1].ReadImagefData();
		pftSubTex[1][2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.2_2.hdr");
		pftSubTex[1][2].ReadImagefData();
		pftSubTex[1][3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.2_3.hdr");
		pftSubTex[1][3].ReadImagefData();
		pftSubTex[1][4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.2_4.hdr");
		pftSubTex[1][4].ReadImagefData();
		pftSubTex[1][5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.2_5.hdr");
		pftSubTex[1][5].ReadImagefData();

		pftSubTex[2][0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.4_0.hdr");
		pftSubTex[2][0].ReadImagefData();
		pftSubTex[2][1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.4_1.hdr");
		pftSubTex[2][1].ReadImagefData();
		pftSubTex[2][2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.4_2.hdr");
		pftSubTex[2][2].ReadImagefData();
		pftSubTex[2][3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.4_3.hdr");
		pftSubTex[2][3].ReadImagefData();
		pftSubTex[2][4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.4_4.hdr");
		pftSubTex[2][4].ReadImagefData();
		pftSubTex[2][5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.4_5.hdr");
		pftSubTex[2][5].ReadImagefData();

		pftSubTex[3][0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.6_0.hdr");
		pftSubTex[3][0].ReadImagefData();
		pftSubTex[3][1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.6_1.hdr");
		pftSubTex[3][1].ReadImagefData();
		pftSubTex[3][2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.6_2.hdr");
		pftSubTex[3][2].ReadImagefData();
		pftSubTex[3][3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.6_3.hdr");
		pftSubTex[3][3].ReadImagefData();
		pftSubTex[3][4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.6_4.hdr");
		pftSubTex[3][4].ReadImagefData();
		pftSubTex[3][5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.6_5.hdr");
		pftSubTex[3][5].ReadImagefData();

		pftSubTex[4][0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.8_0.hdr");
		pftSubTex[4][0].ReadImagefData();
		pftSubTex[4][1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.8_1.hdr");
		pftSubTex[4][1].ReadImagefData();
		pftSubTex[4][2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.8_2.hdr");
		pftSubTex[4][2].ReadImagefData();
		pftSubTex[4][3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.8_3.hdr");
		pftSubTex[4][3].ReadImagefData();
		pftSubTex[4][4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.8_4.hdr");
		pftSubTex[4][4].ReadImagefData();
		pftSubTex[4][5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r0.8_5.hdr");
		pftSubTex[4][5].ReadImagefData();

		pftSubTex[5][0].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r1.0_0.hdr");
		pftSubTex[5][0].ReadImagefData();
		pftSubTex[5][1].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r1.0_1.hdr");
		pftSubTex[5][1].ReadImagefData();
		pftSubTex[5][2].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r1.0_2.hdr");
		pftSubTex[5][2].ReadImagefData();
		pftSubTex[5][3].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r1.0_3.hdr");
		pftSubTex[5][3].ReadImagefData();
		pftSubTex[5][4].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r1.0_4.hdr");
		pftSubTex[5][4].ReadImagefData();
		pftSubTex[5][5].SetImagePath("..\\PictureSource\\HDRi\\music_hall_01_4k\\PS_pftSub\\Prefilter_r1.0_5.hdr");
		pftSubTex[5][5].ReadImagefData();
	}

	// BRDF积分贴图
	lutTex.SetImagePath("..\\PictureSource\\lut.hdr");
	lutTex.ReadImagefData();
}

void CSkybox::DrawSamplePoint(CDC* pDC, CProjection* camera)
{
	CVector3 up = CVector3(0.0, 1.0, 0.0);
	CVector3 normal = CVector3(0.0, -1.0, 0.0).Normalized();// N
	CVector3 right = CrossProduct(up, normal);// T
	if (right.Norm() <= 1e-4)//若right为零向量
	{
		right.x = 1.0, right.y = 1.0, right.z = -(normal.x + normal.y) / (normal.z + 0.001);
		right.Normalize();
	}
	CVector3 front = CrossProduct(normal, right).Normalized();// B

	float sampleDelta = 0.09;// 球面采样步长
	float nrSamples = 0.0;// 总采样数量
	for (float phi = 0; phi < 2 * PI; phi += sampleDelta)
	{
		for (float theta = 0; theta < PI / 2; theta += sampleDelta)
		{
			//spherical to cartesian (in tangent space)
			CVector3 tanSampleVec = CVector3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));// phi从T方向开始，绕N正向顺时针移动
			// tangent space to world space
			CVector3 sampleVec = CVector3(
				DotProduct(CVector3(right.x, front.x, normal.x), tanSampleVec),
				DotProduct(CVector3(right.y, front.y, normal.y), tanSampleVec),
				DotProduct(CVector3(right.z, front.z, normal.z), tanSampleVec));

			CP3 samplePoint = CP3(sampleVec.x * 50, sampleVec.y * 50, sampleVec.z * 50);
			CP3 screenP = camera->PerspectiveProjection(samplePoint);
			CRGB color = CRGB(1.0, 1.0, 1.0) * sin(theta);
			pDC->SetPixelV(screenP.x, screenP.y, CRGBtoRGB(color));
		}
	}
}

void CSkybox::UnfoldSkybox(CDC* pDC)
{
	int e = 0;
	envSubTex[0].ShowTexture(256 + e, 0, pDC);
	envSubTex[1].ShowTexture(-768 + e, 0, pDC);
	envSubTex[2].ShowTexture(-256 + e, 512, pDC);
	envSubTex[3].ShowTexture(-256 + e, -512, pDC);
	envSubTex[4].ShowTexture(768 + e, 0, pDC);
	envSubTex[5].ShowTexture(-256 + e, 0, pDC);
}

void CSkybox::UnfoldIrrBox(CDC* pDC)
{
	int e = 0;
	//irrSubTex[0].ShowTexture(256 + e, 0, 512, 512, pDC);
	//irrSubTex[1].ShowTexture(-768 + e, 0, 512, 512, pDC);
	//irrSubTex[2].ShowTexture(-256 + e, 512, 512, 512, pDC);
	//irrSubTex[3].ShowTexture(-256 + e, -512, 512, 512, pDC);
	//irrSubTex[4].ShowTexture(768 + e, 0, 512, 512, pDC);
	//irrSubTex[5].ShowTexture(-256 + e, 0, 512, 512, pDC);
	irrSubTex[0].ShowTexture(32 + e, 0, pDC);
	irrSubTex[1].ShowTexture(-96 + e, 0, pDC);
	irrSubTex[2].ShowTexture(-32 + e, 64, pDC);
	irrSubTex[3].ShowTexture(-32 + e, -64, pDC);
	irrSubTex[4].ShowTexture(96 + e, 0, pDC);
	irrSubTex[5].ShowTexture(-32 + e, 0, pDC);
}

void CSkybox::UnfoldPftBox(CDC* pDC)
{
	int pftLevel = 0;
	int e = 0;
	pftSubTex[pftLevel][0].ShowTexture(32 + e, 0, pDC);
	pftSubTex[pftLevel][1].ShowTexture(-96 + e, 0, pDC);
	pftSubTex[pftLevel][2].ShowTexture(-32 + e, 64, pDC);
	pftSubTex[pftLevel][3].ShowTexture(-32 + e, -64, pDC);
	pftSubTex[pftLevel][4].ShowTexture(96 + e, 0, pDC);
	pftSubTex[pftLevel][5].ShowTexture(-32 + e, 0, pDC);
}

void CSkybox::OutPutpftBox()
{
	//pftSubTex[0][0].OutputImage()
}

float CSkybox::GeometrySchlickGGX_IBL(float NdotV, float roughness)
{
	float r = roughness;
	float k = (r * r) / 2.0;// IBL光照的k

	float nom = NdotV;
	float denom = NdotV * (1 - k) + k;

	return nom / denom;
}

float CSkybox::GeometrySmith_IBL(CVector3 normal, CVector3 viewVector, CVector3 lightVector, float roughness)
{
	float NdotV = DotProduct(normal, viewVector);
	float NdotL = DotProduct(normal, lightVector);

	float ggx2 = GeometrySchlickGGX_IBL(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX_IBL(NdotL, roughness);

	return ggx1 * ggx2;
}

