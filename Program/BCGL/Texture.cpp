#include "pch.h"
#include "Texture.h"
#define ROUND(d) int(floor(d+0.5))
#define CRGBtoRGB(c) RGB(c.red*255,c.green*255,c.blue*255)

CTexture::CTexture(void)
{
	imgData = NULL;
	imgfData = NULL;
	mipmap = NULL;
	isHDRi = FALSE;
}

CTexture::~CTexture(void)
{
	if (imgData != NULL)
	{
		delete[] imgData;
		imgData = NULL;
	}
	if (imgfData != NULL)
	{
		delete[] imgfData;
		imgfData = NULL;
	}
	for (int nLevel = 0; nLevel < nTotalLevel; nLevel++)
	{
		if (mipmap[nLevel] != NULL)
		{
			delete[] mipmap[nLevel];
			mipmap[nLevel] = NULL;
		}
	}
	if (mipmap != NULL)
	{
		delete[] mipmap;
		mipmap = NULL;
	}
}

void CTexture::CreateImgData(int imgWidth, int imgHeight, int nChannel)
{
	this->imgWidth = imgWidth, this->imgHeight = imgHeight, this->nChannel = nChannel;
	imgfData = new float[imgWidth * imgHeight * nChannel];
}

void CTexture::SetImagePath(string imgPath)
{
	this->imgPath = imgPath;
}

void CTexture::ReadImagefData()
{
	isHDRi = TRUE;
	imgfData = stbi_loadf(imgPath.c_str(), &imgWidth, &imgHeight, &nChannel, 0);// 获取图像颜色数据
}

void CTexture::ReadImageData()
{
	isHDRi = FALSE;
	imgData = stbi_load(imgPath.c_str(), &imgWidth, &imgHeight, &nChannel, 0);// 获取图像颜色数据
}

CRGB CTexture::SampleTexture(CT2 textureCoord)
{
	float u = textureCoord.u, v = textureCoord.v;
	int realU = ROUND((imgWidth - 1) * u);
	int realV = ROUND((imgHeight - 1) * (1 - v));
	// 环绕方式 Clamp to Edge
	if (realU < 0) realU = 0;
	if (realU > (imgWidth - 1)) realU = imgWidth - 1;
	if (realV < 0) realV = 0;
	if (realV > (imgHeight - 1)) realV = imgHeight - 1;

	int dataPosition;// 纹素坐标在imgData数组中相应的位置
	dataPosition = (realU + realV * imgWidth) * nChannel;

	if (isHDRi)
	{
		CRGB color(imgfData[dataPosition], imgfData[dataPosition + 1], imgfData[dataPosition + 2]);
		return color;
	}
	else
	{
		CRGB color(imgData[dataPosition]/255.f, imgData[dataPosition + 1]/255.f, imgData[dataPosition + 2]/255.f);
		return color;
	}
}

CRGB CTexture::SampleTextureLF(CT2 textureCoord)
{
	float u = textureCoord.u, v = textureCoord.v;
	float realU = ((imgWidth - 1) * u);
	float realV = ((imgHeight - 1) * (1 - v));
	// 环绕方式 Clamp to Edge
	if (realU < 0) realU = 0;
	if (realU > (imgWidth - 1)) realU = imgWidth - 1;
	if (realV < 0) realV = 0;
	if (realV > (imgHeight - 1)) realV = imgHeight - 1;
	// 过滤
	int dataPosition[3];// 纹素坐标在imgData数组中相应的位置
	dataPosition[0] = (floor(realU) + floor(realV) * imgWidth) * nChannel;
	if (ROUND(realU) >= realU)
		dataPosition[1] = (ClamptoEdge(ceil(realU), imgWidth) + ClamptoEdge(floor(realV), imgHeight) * imgWidth) * nChannel;
	if (ROUND(realU) < realU)
		dataPosition[1] = (ClamptoEdge(floor(realU - 1),imgWidth) + ClamptoEdge(floor(realV),imgHeight) * imgWidth) * nChannel;
	if (ROUND(realV) >= realV)
		dataPosition[2] = (ClamptoEdge(floor(realU),imgWidth) + ClamptoEdge(ceil(realV),imgHeight) * imgWidth) * nChannel;
	if (ROUND(realV) < realV)
		dataPosition[2] = (ClamptoEdge(floor(realU),imgWidth) + ClamptoEdge(floor(realV - 1),imgHeight) * imgWidth) * nChannel;

	if (isHDRi)
	{
		CRGB color[3];
		for (int i = 0; i < 3; i++)
			color[i] = CRGB(imgfData[dataPosition[i]], imgfData[dataPosition[i] + 1], imgfData[dataPosition[i] + 2]);
		float dU0, dU1;
		float dV0, dV1;
		dU0 = fabs(realU - floor(realU) - 0.5);
		if (ROUND(realU) >= realU)
			dU1 = ceil(realU) - realU + 0.5;
		if (ROUND(realU) < realU)
			dU1 = realU - floor(realU) + 0.5;
		dV0 = fabs(realV - floor(realV) - 0.5);
		if(ROUND(realV)>=realV)
			dV1 = ceil(realV) - realV + 0.5;
		if (ROUND(realV) < realV)
			dU1 = realV - floor(realV) + 0.5;

		CRGB resultColor = (color[0] * dU0 / (dU0 + dU1) + color[1] * dU1 / (dU0 + dU1)) * dV0 / (dV0 + dV1) + color[2] * dV1 / (dV0 + dV1);
		return resultColor;
	}
	else
	{
		CRGB color[3];
		for (int i = 0; i < 3; i++)
			color[i] = CRGB(imgData[dataPosition[i]], imgData[dataPosition[i] + 1], imgData[dataPosition[i] + 2]);
		float dU0, dU1;
		float dV0, dV1;
		dU0 = fabs(realU - floor(realU) - 0.5);
		if (ROUND(realU) >= realU)
			dU1 = ceil(realU) - realU + 0.5;
		if (ROUND(realU) < realU)
			dU1 = realU - floor(realU) + 0.5;
		dV0 = fabs(realV - floor(realV) - 0.5);
		if (ROUND(realV) >= realV)
			dV1 = ceil(realV) - realV + 0.5;
		if (ROUND(realV) < realV)
			dV1 = realV - floor(realV) + 0.5;

		CRGB resultColor = (color[0] * dU0 / (dU0 + dU1) + color[1] * dU1 / (dU0 + dU1)) * dV0 / (dV0 + dV1) + color[2] * dV1 / (dV0 + dV1);
		resultColor /= 255.f;
		return resultColor;
	}
}

int CTexture::ClamptoEdge(int uv, int range)
{
	if (uv < 0) uv = 0;
	if (uv > (range - 1)) uv = range - 1;
	return uv;
}

void CTexture::ShowTexture(CDC* pDC)
{
	if (imgfData != NULL)
	{
		for (int y = 0; y < imgHeight; y++)
		{
			for (int x = 0; x < imgWidth; x++)
			{
				int position = (x + y * imgWidth) * nChannel;
				CRGB color(imgfData[position], imgfData[position + 1], imgfData[position + 2]);
				color.Exposure(1.0);
				pDC->SetPixelV(x - imgWidth / 2, 1 - (y - imgHeight / 2), CRGBtoRGB(color));
			}
		}
	}
}

void CTexture::ShowTexture(int scrX, int scrY, CDC* pDC)
{
	if (imgfData != NULL)
	{
		for (int y = 0; y < imgHeight; y++)
		{
			for (int x = 0; x < imgWidth; x++)
			{
				int position = (x + y * imgWidth) * nChannel;
				CRGB color(imgfData[position], imgfData[position + 1], imgfData[position + 2]);
				color.Exposure(1.0);
				pDC->SetPixelV(x - imgWidth / 2 + scrX, 1 - (y - imgHeight / 2) + scrY, CRGBtoRGB(color));
			}
		}
	}
}

void CTexture::ShowTexture(int scrX, int scrY, int showWidth, int showHeight, CDC* pDC)
{
	if (imgfData != NULL)
	{
		int scale = showWidth / imgWidth;// 缩放倍数
		for (int y = 0; y < showHeight; y++)
		{
			for (int x = 0; x < showWidth; x++)
			{
				int position = (x % scale + y % scale * imgWidth) * nChannel;
				CRGB color(imgfData[position], imgfData[position + 1], imgfData[position + 2]);
				color.Exposure(1.0);
				//color.Reinhard();
				pDC->SetPixelV(x - showWidth / 2 + scrX, 1 - (y - showHeight / 2) + scrY, CRGBtoRGB(color));
			}
		}
	}
}

void CTexture::FreeStbImg()
{
	stbi_image_free(imgfData);
}

void CTexture::GenMipMap(int nTotalLevel)
{
	this->nTotalLevel = nTotalLevel;
	mipmap = new float* [nTotalLevel];// 根据总mipmap级数确定新增的mipmap数量
	int curWidth = imgWidth, curHeight = imgHeight;// 各级别mipmap的分辨率
	for (int nLevel = 0; nLevel < nTotalLevel; nLevel++)
	{
		mipmap[nLevel] = new float[curWidth * curHeight * nChannel];
		curWidth = curWidth / 2, curHeight = curHeight / 2;
	}

	memcpy(mipmap[0], imgfData, imgWidth * imgHeight * nChannel * sizeof(float));// 将原图拷贝到Level0的mipmap中
	curWidth = imgWidth / 2, curHeight = imgHeight / 2;
	for (int nLevel = 1; nLevel < nTotalLevel; nLevel++)
	{
		curWidth = imgWidth / pow(2, nLevel), curHeight = imgHeight / pow(2, nLevel);
		for (int v = 0; v < curHeight; v++)
		{
			for (int u = 0; u < curWidth; u++)
			{
				int selfPos = (u + v * curWidth) * nChannel;
				int fatherPos = (2 * u + 2 * v * (2 * curWidth)) * nChannel;// 父级mipmap采样位置基准点，坐标变2倍，宽度也要变2倍

				CRGB fColor0 = CRGB(mipmap[nLevel - 1][fatherPos], mipmap[nLevel - 1][fatherPos + 1], mipmap[nLevel - 1][fatherPos + 2]);
				CRGB fColor1 = CRGB(mipmap[nLevel - 1][fatherPos + 3], mipmap[nLevel - 1][fatherPos + 4], mipmap[nLevel - 1][fatherPos + 5]);
				CRGB fColor2 = CRGB(mipmap[nLevel - 1][fatherPos + (2 * curWidth * nChannel)], mipmap[nLevel - 1][fatherPos + 1 + (2 * curWidth * nChannel)], mipmap[nLevel - 1][fatherPos + 2 + (2 * curWidth * nChannel)]);
				CRGB fColor3 = CRGB(mipmap[nLevel - 1][fatherPos + 3 + (2 * curWidth * nChannel)], mipmap[nLevel - 1][fatherPos + 4 + (2 * curWidth * nChannel)], mipmap[nLevel - 1][fatherPos + 5 + (2 * curWidth * nChannel)]);

				CRGB result = (fColor0 + fColor1 + fColor2 + fColor3) / 4;
				mipmap[nLevel][selfPos] = result.red;
				mipmap[nLevel][selfPos + 1] = result.green;
				mipmap[nLevel][selfPos + 2] = result.blue;
			}
		}
	}
}

void CTexture::ShowMipMap(CDC* pDC)
{
	int showCenterX = 0;// 展示中心
	for (int nLevel = 0; nLevel < nTotalLevel; nLevel++)
	{
		if (mipmap[nLevel] != NULL)
		{
			int curWidth = imgWidth / pow(2, nLevel), curHeight = imgHeight / pow(2, nLevel);
			showCenterX += 1000 / pow(2, nLevel);
			for (int y = 0; y < curHeight; y++)
			{
				for (int x = 0; x < curWidth; x++)
				{
					int position = (x + y * curWidth) * nChannel;
					CRGB color(mipmap[nLevel][position], mipmap[nLevel][position + 1], mipmap[nLevel][position + 2]);
					color.Exposure(1.0);
					pDC->SetPixelV(x - curWidth / 2 + showCenterX - 1400, -(y - curHeight / 2), CRGBtoRGB(color));
				}
			}
		}
	}
}

int CTexture::GetImgWidth()
{
	return imgWidth;
}

int CTexture::GetImgHeight()
{
	return imgHeight;
}

int CTexture::GetImgChannel()
{
	return nChannel;
}


CRGB CTexture::SampleSphericalMap(CVector3 vec3)
{
	double u, v;
	u = atan2(vec3.z, vec3.x);// 结果为向量在xOz平面投影与x轴的夹角 phi
	v = asin(vec3.y);// 结果为向量与xOz面的夹角 theta

	u = 0.1591 * u;// 0.1591为2PI的倒数
	v = 0.3183 * v;// 0.3183为PI的倒数

	u += 0.5;
	v += 0.5;

	return SampleTexture(CT2(u, v));
}

void CTexture::OutputImage(string imgName)
{
	string outputPath = "..\\PictureSource";
	outputPath += "\\";
	outputPath += imgName;
	stbi_write_hdr(outputPath.c_str(), imgWidth, imgHeight, nChannel, imgfData);
}
