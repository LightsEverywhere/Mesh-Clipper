#pragma once
#include<string>
#include"RGB.h"
#include"../ImgLoad/stb_image.h"
#include"../ImgLoad/stb_image_write.h"
#include"T2.h"
#include"Vector3.h"
using namespace std;

class CTexture
{
public:
	CTexture(void);
	virtual ~CTexture(void);
	void CreateImgData(int imgWidth,int imgHeight,int nChannel);// 为图像数据数组分配内存

	void SetImagePath(string imgPath);
	void ReadImagefData();
	void ReadImageData();
	CRGB SampleTexture(CT2 textureCoord);
	CRGB SampleTextureLF(CT2 textureCoord);
	int ClamptoEdge(int uv, int range);
	void ShowTexture(CDC* pDC);// 将纹理显示在屏幕上
	void ShowTexture(int scrX, int scrY, CDC* pDC);// 将纹理显示在屏幕指定位置
	void ShowTexture(int scrX, int scrY, int showWidth, int showHeight, CDC* pDC);
	void FreeStbImg();

	void GenMipMap(int nTotalLevel);// 生成mipmap
	void ShowMipMap(CDC* pDC);// 将图像的各级mipmap显示在屏幕上

	int GetImgWidth();// 获取图像宽度
	int GetImgHeight();// 获取图像高度
	int GetImgChannel();// 获取图像通道数

	CRGB SampleSphericalMap(CVector3 vec3);// 将纹理作为等距柱状图获取颜色信息

	void OutputImage(string imgName);// 输出图像

public:
	string imgPath;
	float* imgfData;// 可容纳最大32位色的图像
	unsigned char* imgData;// jpg,png使用
	int imgWidth, imgHeight, nChannel;

	float** mipmap;// 图像对应的多级别mipmap
	int nTotalLevel;// mipmap级数

	BOOL isHDRi;
};

