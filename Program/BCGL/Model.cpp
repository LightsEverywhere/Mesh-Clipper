#include "pch.h"
#include "Model.h"
#include<omp.h>
#define ROUND(d) int(floor(d+0.5))
#define CRGBtoRGB(c) RGB(c.red*255,c.green*255,c.blue*255)

CModel::CModel(void)
{
	fileName = _T("");

	vertex = NULL;
	textureCoord = NULL;
	normal = NULL;
	face = NULL;
	triangle = NULL;

	targetTexture = NULL;
	targetAlbedo = NULL;
	targetNormal = NULL;
	targetMetallic = NULL;
	targetRoughness = NULL;
	targetAO = NULL;

	nTotalVertex = 0;
	nTotalTexture = 0;
	nTotalNormal = 0;
	nTotalFace = 0;
	nTotalTriangle = 0;

	maxY = -10000, maxX = -10000, maxZ = -10000, minY = 10000, minX = 10000, minZ = 10000;
}

CModel::~CModel(void)
{
	if (vertex != NULL)
	{
		delete[] vertex;
		vertex = NULL;
	}
	if (textureCoord != NULL)
	{
		delete[] textureCoord;
		textureCoord = NULL;
	}
	if (normal != NULL)
	{
		delete[] normal;
		normal = NULL;
	}
	if (face != NULL)
	{
		delete[] face;
		face = NULL;
	}
	if (triangle != NULL)
	{
		delete[] triangle;
		triangle = NULL;
	}
}

void CModel::GatherStatistics(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		MessageBoxW(NULL, _T("未找到模型文件！"), _T("警告"), MB_ICONEXCLAMATION);
		return;
	}
	CString strLine;
	// 计算各项信息数量
	while (file.ReadString(strLine))
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')
		{
			nTotalVertex++;
		}
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')
		{
			nTotalTexture++;
		}
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')
		{
			nTotalNormal++;
		}
		if (strLine[0] == 'f' && strLine[1] == ' ')
		{
			nTotalFace++;
		}
	}
	file.Close();
}

void CModel::ReadVertex(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	vertex = new CP3[nTotalVertex];
	int index = 0;// 队列索引
	CString strLine;
	while (file.ReadString(strLine) && index < nTotalVertex)
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')
		{
			if (strLine[2] == ' ')//判断v后是否有两个空格符
			{
				CString str[3];
				for (int i = 0; i < 3; i++)
				{
					AfxExtractSubString(str[i], strLine, i + 2, ' ');
				}
				vertex[index].x = _wtof(str[0]);
				vertex[index].y = _wtof(str[1]);
				vertex[index].z = _wtof(str[2]);
				//更新模型包围盒范围
				if (vertex[index].x > maxX)	maxX = vertex[index].x;
				if (vertex[index].x < minX)	minX = vertex[index].x;
				if (vertex[index].y > maxY)	maxY = vertex[index].y;
				if (vertex[index].y < minY)	minY = vertex[index].y;
				if (vertex[index].z > maxZ)	maxZ = vertex[index].z;
				if (vertex[index].z < minZ)	minZ = vertex[index].z;
				index++;
			}
			if (strLine[2] != ' ')
			{
				CString str[3];
				for (int i = 0; i < 3; i++)
				{
					AfxExtractSubString(str[i], strLine, i + 1, ' ');
				}
				vertex[index].x = _wtof(str[0]);
				vertex[index].y = _wtof(str[1]);
				vertex[index].z = _wtof(str[2]);
				//更新模型包围盒范围
				if (vertex[index].x > maxX)	maxX = vertex[index].x;
				if (vertex[index].x < minX)	minX = vertex[index].x;
				if (vertex[index].y > maxY)	maxY = vertex[index].y;
				if (vertex[index].y < minY)	minY = vertex[index].y;
				if (vertex[index].z > maxZ)	maxZ = vertex[index].z;
				if (vertex[index].z < minZ)	minZ = vertex[index].z;
				index++;
			}
		}
	}
	file.Close();
	modelLength = maxX - minX;
	modelWidth = maxZ - minZ;
	modelHeight = maxY - minY;
	centerPoint = CP3((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);
}

void CModel::ReadTexture(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	textureCoord = new CT2[nTotalTexture];
	CString strLine;
	int index=0;
	while (file.ReadString(strLine) && index < nTotalTexture)
	{
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')
		{
			CString str[2];
			for (int i = 0; i < 2; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
			}
			textureCoord[index].u = _wtof(str[0]);
			textureCoord[index].v = _wtof(str[1]);
			index++;
		}
	}
	file.Close();
}

void CModel::ReadNormal(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	normal = new CVector3[nTotalNormal];
	CString strLine;
	int index = 0;// 队列索引
	while (file.ReadString(strLine) && index < nTotalNormal)
	{
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')
		{
			CString str[3];
			for (int i = 0; i < 3; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
			}
			normal[index].x = _wtof(str[0]);
			normal[index].y = _wtof(str[1]);
			normal[index].z = _wtof(str[2]);
			index++;
		}
	}
	file.Close();
}

void CModel::ReadFace(void)
{
	CStdioFile file;
	if (!file.Open(fileName, CFile::modeRead))
	{
		return;
	}
	face = new CFace[nTotalFace];
	CString strLine;
	int index = 0;// 队列索引
	while (file.ReadString(strLine) && index < nTotalFace)
	{
		if (strLine[0] == 'f' && strLine[1] == ' ')
		{
			// 计算面顶点数量
			int count = 0;// '/'符计数
			for (int i = 0; i < strLine.GetLength(); i++)
			{
				if (strLine[i] == '/')
					count++;
			}
			face[index].vertexNumber = count / 2;// 面顶点数
			face[index].InitializeQueue();// 初始化面的索引队列

			CString* str = new CString[face[index].vertexNumber];

			for (int j = 0; j < face[index].vertexNumber; j++)
			{
				AfxExtractSubString(str[j], strLine, j + 1, ' ');
				CString strs[3];
				for (int k = 0; k < 3; k++)
				{
					AfxExtractSubString(strs[k], str[j], k, '/');
				}
				face[index].vertexIndex[j] = _wtof(strs[0])-1;
				face[index].textureIndex[j] = _wtof(strs[1])-1;
				face[index].normalIndex[j] = _wtof(strs[2])-1;
			}

			delete[] str;
			index++;
		}
	}
	file.Close();
}

void CModel::ReadTriangle(void)
{
	// 计算片元数量
	for (int nFace = 0; nFace < nTotalFace; nFace++)
	{
		if (face[nFace].vertexNumber == 4)
		{
			nTotalTriangle += 2;
		}
		else
		{
			nTotalTriangle += 1;
		}
	}
	// 读取片元队列
	triangle = new CTriangle[nTotalTriangle];
	int index = 0;// 片元队列索引
	for (int nFace = 0; nFace < nTotalFace; nFace++)
	{
		if (face[nFace].vertexNumber == 4)
		{
			// 传索引
			for (int i = 0; i < 3; i++)
			{
				triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i];
				triangle[index].textureIndex[i] = face[nFace].textureIndex[i];
				triangle[index].normalIndex[i] = face[nFace].normalIndex[i];
			}
			index++;
			for (int i = 0; i < 3; i++)
			{
				if (i == 0)
				{
					triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i];
					triangle[index].textureIndex[i] = face[nFace].textureIndex[i];
					triangle[index].normalIndex[i] = face[nFace].normalIndex[i];
				}
				else
				{
					triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i + 1];
					triangle[index].textureIndex[i] = face[nFace].textureIndex[i + 1];
					triangle[index].normalIndex[i] = face[nFace].normalIndex[i + 1];
				}
			}
			index++;
		}
		if (face[nFace].vertexNumber == 3)
		{
			for (int i = 0; i < 3; i++)
			{
				triangle[index].vertexIndex[i] = face[nFace].vertexIndex[i];
				triangle[index].textureIndex[i] = face[nFace].textureIndex[i];
				triangle[index].normalIndex[i] = face[nFace].normalIndex[i];
			}
			index++;
		}
	}
}

void CModel::GetScreenBox(void)
{
	xLeft = 10000, xRight = -10000, yTop = -10000, yBottom = 10000;// 初始化包围盒范围
	CP3 screenP;
	for (int nVertex = 0; nVertex < nTotalVertex; nVertex++)
	{
		screenP = camera->PerspectiveProjection(vertex[nVertex]);
		//更新屏幕包围盒
		if (screenP.x < xLeft) xLeft = int(floor(screenP.x));
		if (screenP.x > xRight) xRight = int(ceil(screenP.x));
		if (screenP.y < yBottom) yBottom = int(floor(screenP.y));
		if (screenP.y > yTop) yTop = int(ceil(screenP.y));
	}
	screenH = max(abs(yTop), abs(yBottom))*2;
	screenW = max(abs(xRight), abs(xLeft))*2;
}

CTriangle* CModel::GetTriangle()
{
	for (int nTri = 0; nTri < nTotalTriangle; nTri++)
	{
		for (int i = 0; i < 3; i++)
		{
			triangle[nTri].P[i] = vertex[triangle[nTri].vertexIndex[i]];
		}

	}
	return triangle;
}

void CModel::BindTexture(CTexture* targetTexture)
{
	this->targetTexture = targetTexture;
}

void CModel::BindAlbedo(CTexture* targetAlbedo)
{
	this->targetAlbedo = targetAlbedo;
}

void CModel::BindNormal(CTexture* targetNormal)
{
	this->targetNormal = targetNormal;
}

void CModel::BindMetallic(CTexture* targetMetallic)
{
	this->targetMetallic = targetMetallic;
}

void CModel::BindRoughness(CTexture* targetRoughness)
{
	this->targetRoughness = targetRoughness;
}

void CModel::BindAO(CTexture* targetAO)
{
	this->targetAO = targetAO;
}

void CModel::SetMaterial(CMaterial* material)
{
	this->material = material;
}

void CModel::SetScene(CLighting* scene)
{
	this->scene = scene;
}

void CModel::SetZBuffer(CZBuffer* zbuffer)
{
	this->zbuffer = zbuffer;
}

bool CModel::InsideTriangle(float x, float y, const CP3* point)
{
	CVector3 v[3];
	v[0] = CVector3(point[0], point[1]).Normalized();
	v[1] = CVector3(point[1], point[2]).Normalized();
	v[2] = CVector3(point[2], point[0]).Normalized();
	CVector3 f[3];
	for (int i = 0; i < 3; i++)
	{
		f[i] = CVector3(point[i], CP2(x, y)).Normalized();
	}
	CVector3 n[3];
	n[0] = CrossProduct(v[0], f[0]);
	n[1] = CrossProduct(v[1], f[1]);
	n[2] = CrossProduct(v[2], f[2]);

	if (n[0].z*n[1].z>=-1e-10&& n[1].z * n[2].z >= -1e-10 && n[2].z * n[0].z >= -1e-10)
		return true;
	else
		return false;
}

void CModel::DrawPointCloud(CDC* pDC)
{
	CP3 screenP;
	for (int nVertex = 0; nVertex < nTotalVertex; nVertex++)
	{
		screenP = camera->PerspectiveProjection(vertex[nVertex]);
		pDC->SetPixelV(ROUND(screenP.x), ROUND(screenP.y), RGB(255, 255, 255));
	}
	omp_set_num_threads(3);
	
	//for (int x = -250; x < 250; x++)
//	{
//#pragma omp parallel for schedule(static)
//		for (int y = 0; y < 250; y++)
//		{
//			pDC->SetPixelV(0, y, RGB(255, 0, 0));
//		}
//	}
}

void CModel::DrawWireframe(CDC* pDC)
{
	CP3 screenP;
	CPen newPen, * pOldPen;
	newPen.CreatePen(BS_SOLID, 1, RGB(100, 100, 100));
	pDC->SelectObject(&newPen);
	for (int nFace = 0; nFace < nTotalFace; nFace++)// 面循环
	{
		CVector3 edgeVector01 = CVector3(vertex[face[nFace].vertexIndex[0]], vertex[face[nFace].vertexIndex[1]]).Normalized();
		CVector3 edgeVector02 = CVector3(vertex[face[nFace].vertexIndex[0]], vertex[face[nFace].vertexIndex[2]]).Normalized();
		CVector3 faceNormal = CrossProduct(edgeVector01, edgeVector02);
		CVector3 viewVector = CVector3(vertex[face[nFace].vertexIndex[0]], camera->GetEye()).Normalized();
		//if (DotProduct(viewVector, faceNormal) >= 0)
		{
			screenP = camera->PerspectiveProjection(vertex[face[nFace].vertexIndex[0]]);
			pDC->MoveTo(ROUND(screenP.x), ROUND(screenP.y));
			for (int nPoint = 1; nPoint < face[nFace].vertexNumber; nPoint++)
			{
				screenP = camera->PerspectiveProjection(vertex[face[nFace].vertexIndex[nPoint]]);
				pDC->LineTo(ROUND(screenP.x), ROUND(screenP.y));
			}
			screenP = camera->PerspectiveProjection(vertex[face[nFace].vertexIndex[0]]);
			pDC->LineTo(ROUND(screenP.x), ROUND(screenP.y));
		}
	}
}

void CModel::BlinnPhongShader(CDC* pDC)
{
	//#pragma omp parallel for
	for (int nTriangle = 0; nTriangle < nTotalTriangle; nTriangle++)
	{
		// 边向量
		CVector3 sideVector01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized();
		CVector3 sideVector02 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized();
		CVector3 triangleNormal = CrossProduct(sideVector01, sideVector02).Normalized();// 图元（面）法向
		CVector3 viewVector = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], camera->GetEye()).Normalized();// 图元视向量

		if (DotProduct(viewVector, triangleNormal) >= 0)// 背面剔除
		{
			// 求TBN矩阵
			CVector3 TBN[3];// TBN矩阵
			if (targetNormal != NULL)
			{
				// UV坐标差
				double deltaU1, deltaV1, deltaU2, deltaV2;
				deltaU1 = (textureCoord[triangle[nTriangle].textureIndex[1]] - textureCoord[triangle[nTriangle].textureIndex[0]]).u;
				deltaV1 = (textureCoord[triangle[nTriangle].textureIndex[1]] - textureCoord[triangle[nTriangle].textureIndex[0]]).v;
				deltaU2 = (textureCoord[triangle[nTriangle].textureIndex[2]] - textureCoord[triangle[nTriangle].textureIndex[1]]).u;
				deltaV2 = (textureCoord[triangle[nTriangle].textureIndex[2]] - textureCoord[triangle[nTriangle].textureIndex[1]]).v;

				// 切线空间各坐标分量
				CVector3 T, B, N;
				CVector3 edge01, edge02;
				edge01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized() * sqrt(deltaU1 * deltaU1 + deltaV1 * deltaV1);
				edge02 = CVector3(vertex[triangle[nTriangle].vertexIndex[1]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized() * sqrt(deltaU2 * deltaU2 + deltaV2 * deltaV2);
				T = ((deltaV2 * edge01 - deltaV1 * edge02) / (deltaU1 * deltaV2 - deltaU2 * deltaV1)).Normalized();//理论上可以不做单位化
				B = ((deltaU1 * edge02 - deltaU2 * edge01) / (deltaU1 * deltaV2 - deltaU2 * deltaV1)).Normalized();
				N = triangleNormal;

				TBN[0] = T, TBN[1] = B, TBN[2] = N;
			}

			CP3 point[3];
			for (int j = 0; j < 3; j++)
			{
				point[j] = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[j]]);
			}
			// 包围盒
			int Xmax = max(ceil(point[0].x), max(ceil(point[1].x), ceil(point[2].x)));
			int Xmin = min(floor(point[0].x), min(floor(point[1].x), floor(point[2].x)));
			int Ymax = max(ceil(point[0].y), max(ceil(point[1].y), ceil(point[2].y)));
			int Ymin = min(floor(point[0].y), min(floor(point[1].y), floor(point[2].y)));

			for (int x = Xmin; x < Xmax; x++)
			{
				for (int y = Ymin; y < Ymax; y++)
				{
					if ((x + zbuffer->nWidth / 2) < zbuffer->nWidth && (y + zbuffer->nHeight / 2) < zbuffer->nHeight
						&& (x + zbuffer->nWidth / 2) >= 0 && (y + zbuffer->nHeight / 2) >= 0)// zbuffer适应调整
					{
						if (InsideTriangle((float)x + 0.5, (float)y + 0.5, point))
						{
							double Area = point[0].x * (point[1].y - point[2].y) + point[1].x * (point[2].y - point[0].y) + point[2].x * (point[0].y - point[1].y);
							double Area0 = x * (point[1].y - point[2].y) + point[1].x * (point[2].y - y) + point[2].x * (y - point[1].y);
							double Area1 = point[0].x * (y - point[2].y) + x * (point[2].y - point[0].y) + point[2].x * (point[0].y - y);
							double Area2 = point[0].x * (point[1].y - y) + point[1].x * (y - point[0].y) + x * (point[0].y - point[1].y);
							double alpha, beta, gamma;// 重心坐标
							alpha = Area0 / Area, beta = Area1 / Area, gamma = Area2 / Area;
							// 当前点伪深度
							double depth = point[0].z * alpha + point[1].z * beta + point[2].z * gamma;
							if (depth <= zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2])
							{
								zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2] = depth;

								// 透视矫正
								double correctAlpha, correctBeta, correctGamma;// 矫正参数
								correctAlpha = alpha / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[0]]).z;
								correctBeta = beta / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[1]]).z;
								correctGamma = gamma / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[2]]).z;
								// 当前点的世界空间坐标
								CP3 currentWorldPoint;
								currentWorldPoint = (
									correctAlpha * vertex[triangle[nTriangle].vertexIndex[0]]
									+ correctBeta * vertex[triangle[nTriangle].vertexIndex[1]]
									+ correctGamma * vertex[triangle[nTriangle].vertexIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);
								// 当前点世界空间法向
								CVector3 currentNormal;
								currentNormal = ((
									correctAlpha * normal[triangle[nTriangle].normalIndex[0]]
									+ correctBeta * normal[triangle[nTriangle].normalIndex[1]]
									+ correctGamma * normal[triangle[nTriangle].normalIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma)).Normalized();
								// 当前点的世界空间纹理坐标
								CT2 t = (
									correctAlpha * textureCoord[triangle[nTriangle].textureIndex[0]]
									+ correctBeta * textureCoord[triangle[nTriangle].textureIndex[1]]
									+ correctGamma * textureCoord[triangle[nTriangle].textureIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);

								if (targetTexture != NULL)// 若纹理存在
								{
									CRGB diffuseColor = targetTexture->SampleTexture(t);
									material->SetDiffuseRef(diffuseColor);
								}

								CRGB I = scene->SimpleIlluminate(currentWorldPoint, camera->GetEye(), currentNormal, material);
								I.Reinhard();
								//I.Normalize();
								pDC->SetPixelV(x, y, CRGBtoRGB(I));
							}
						}
					}
				}
			}
		}

	}
}

void CModel::PBRendering(CDC* pDC)
{
	for (int nTriangle = 0; nTriangle < nTotalTriangle; nTriangle++)
	{
		// 边向量
		CVector3 sideVector01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized();
		CVector3 sideVector02 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized();
		CVector3 triangleNormal = CrossProduct(sideVector01, sideVector02).Normalized();// 图元（面）法向
		CVector3 viewVector = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], camera->GetEye()).Normalized();// 图元视向量

		if (DotProduct(viewVector, triangleNormal) >= 0)// 背面剔除
		{
			// 求TBN矩阵
			CVector3 TBN[3];// TBN矩阵
			if (targetNormal != NULL)
			{
				// UV坐标差
				double deltaU1, deltaV1, deltaU2, deltaV2;
				deltaU1 = (textureCoord[triangle[nTriangle].textureIndex[1]] - textureCoord[triangle[nTriangle].textureIndex[0]]).u;
				deltaV1 = (textureCoord[triangle[nTriangle].textureIndex[1]] - textureCoord[triangle[nTriangle].textureIndex[0]]).v;
				deltaU2 = (textureCoord[triangle[nTriangle].textureIndex[2]] - textureCoord[triangle[nTriangle].textureIndex[1]]).u;
				deltaV2 = (textureCoord[triangle[nTriangle].textureIndex[2]] - textureCoord[triangle[nTriangle].textureIndex[1]]).v;

				// 切线空间各坐标分量
				CVector3 T, B, N;
				CVector3 edge01, edge02;
				edge01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized() * sqrt(deltaU1 * deltaU1 + deltaV1 * deltaV1);
				edge02 = CVector3(vertex[triangle[nTriangle].vertexIndex[1]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized() * sqrt(deltaU2 * deltaU2 + deltaV2 * deltaV2);
				T = ((deltaV2 * edge01 - deltaV1 * edge02) / (deltaU1 * deltaV2 - deltaU2 * deltaV1)).Normalized();//理论上可以不做单位化
				B = ((deltaU1 * edge02 - deltaU2 * edge01) / (deltaU1 * deltaV2 - deltaU2 * deltaV1)).Normalized();
				N = triangleNormal;

				TBN[0] = T, TBN[1] = B, TBN[2] = N;
			}

			CP3 point[3];
			for (int j = 0; j < 3; j++)
			{
				point[j] = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[j]]);
			}
			// 包围盒
			int Xmax = max(ceil(point[0].x), max(ceil(point[1].x), ceil(point[2].x)));
			int Xmin = min(floor(point[0].x), min(floor(point[1].x), floor(point[2].x)));
			int Ymax = max(ceil(point[0].y), max(ceil(point[1].y), ceil(point[2].y)));
			int Ymin = min(floor(point[0].y), min(floor(point[1].y), floor(point[2].y)));

			for (int x = Xmin; x < Xmax; x++)
			{
				for (int y = Ymin; y < Ymax; y++)
				{
					if ((x + zbuffer->nWidth / 2) < zbuffer->nWidth && (y + zbuffer->nHeight / 2) < zbuffer->nHeight
						&& (x + zbuffer->nWidth / 2) >= 0 && (y + zbuffer->nHeight / 2) >= 0)// zbuffer适应调整
					{
						if (InsideTriangle((float)x + 0.5, (float)y + 0.5, point))
						{
							double Area = point[0].x * (point[1].y - point[2].y) + point[1].x * (point[2].y - point[0].y) + point[2].x * (point[0].y - point[1].y);
							double Area0 = x * (point[1].y - point[2].y) + point[1].x * (point[2].y - y) + point[2].x * (y - point[1].y);
							double Area1 = point[0].x * (y - point[2].y) + x * (point[2].y - point[0].y) + point[2].x * (point[0].y - y);
							double Area2 = point[0].x * (point[1].y - y) + point[1].x * (y - point[0].y) + x * (point[0].y - point[1].y);
							double alpha, beta, gamma;// 重心坐标
							alpha = Area0 / Area, beta = Area1 / Area, gamma = Area2 / Area;
							// 当前点伪深度
							double depth = point[0].z * alpha + point[1].z * beta + point[2].z * gamma;
							if (depth <= zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2])
							{
								zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2] = depth;

								// 透视矫正
								double correctAlpha, correctBeta, correctGamma;// 矫正参数
								correctAlpha = alpha / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[0]]).z;
								correctBeta = beta / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[1]]).z;
								correctGamma = gamma / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[2]]).z;
								// 当前点的世界空间坐标
								CP3 currentWorldPoint;
								currentWorldPoint = (
									correctAlpha * vertex[triangle[nTriangle].vertexIndex[0]]
									+ correctBeta * vertex[triangle[nTriangle].vertexIndex[1]]
									+ correctGamma * vertex[triangle[nTriangle].vertexIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);
								// 当前点世界空间法向
								CVector3 currentNormal;
								currentNormal = ((
									correctAlpha * normal[triangle[nTriangle].normalIndex[0]]
									+ correctBeta * normal[triangle[nTriangle].normalIndex[1]]
									+ correctGamma * normal[triangle[nTriangle].normalIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma)).Normalized();
								// 当前点的世界空间纹理坐标
								CT2 t = (
									correctAlpha * textureCoord[triangle[nTriangle].textureIndex[0]]
									+ correctBeta * textureCoord[triangle[nTriangle].textureIndex[1]]
									+ correctGamma * textureCoord[triangle[nTriangle].textureIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);

								if (targetAlbedo != NULL)
								{
									CRGB albedo = targetAlbedo->SampleTextureLF(t);
									material->SetAlbedo(albedo);
									material->UpdateF0();
								}
								if (targetNormal != NULL)
								{
									CRGB sampColor = targetNormal->SampleTextureLF(t);
									sampColor *= 2.0;
									sampColor -= 1.0;
									CVector3 sampNormal;
									sampNormal.x = sampColor.red;
									sampNormal.y = sampColor.green;
									sampNormal.z = sampColor.blue;

									CVector3 T = TBN[0];
									CVector3 N = currentNormal;
									T = (T - DotProduct(T, N) * N).Normalized();
									CVector3 B = CrossProduct(N, T);

									TBN[0] = T, TBN[1] = B, TBN[2] = N;

									currentNormal = CVector3(
										TBN[0].x * sampNormal.x + TBN[1].x * sampNormal.y + TBN[2].x * sampNormal.z,
										TBN[0].y * sampNormal.x + TBN[1].y * sampNormal.y + TBN[2].y * sampNormal.z,
										TBN[0].z * sampNormal.x + TBN[1].z * sampNormal.y + TBN[2].z * sampNormal.z
									).Normalized();
								}
								if (targetMetallic != NULL)
								{
									CRGB metallic = targetMetallic->SampleTextureLF(t);
									material->SetMetallic(metallic.red);
									material->UpdateF0();
								}
								if (targetRoughness != NULL)
								{
									CRGB roughness = targetRoughness->SampleTextureLF(t);
									material->SetRoughness(roughness.red);
									//material->UpdateF0();
								}

								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.16) material->metallic = 0.0;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.16 && ((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.38) material->metallic = 0.25;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.38 && ((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.58) material->metallic = 0.5;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.58 && ((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.80) material->metallic = 0.75;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.80) material->metallic = 0.95;

								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.16) material->roughness = 1.0;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.16 && ((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.38) material->roughness = 0.75;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.38 && ((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.58) material->roughness = 0.5;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.58 && ((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.80) material->roughness = 0.25;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.80) material->roughness = 0.05;

								CRGB I = scene->PBR(currentWorldPoint, material, currentNormal);
								I.Reinhard();
								//I.Normalize();
								pDC->SetPixelV(x, y, CRGBtoRGB(I));
							}
						}
					}
				}
			}
		}
	}
}

void CModel::PBRenderingwithIBL(CDC* pDC)
{
	//#pragma omp parallel for
	for (int nTriangle = 0; nTriangle < nTotalTriangle; nTriangle++)
	{
		// 边向量
		CVector3 sideVector01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized();
		CVector3 sideVector02 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized();
		CVector3 triangleNormal = CrossProduct(sideVector01, sideVector02).Normalized();// 图元（面）法向
		CVector3 viewVector = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], camera->GetEye()).Normalized();// 图元视向量

		if (DotProduct(viewVector, triangleNormal) >= 0)// 背面剔除
		{
			// 求TBN矩阵
			CVector3 TBN[3];// TBN矩阵
			if (targetNormal != NULL)
			{
				// UV坐标差
				double deltaU1, deltaV1, deltaU2, deltaV2;
				deltaU1 = (textureCoord[triangle[nTriangle].textureIndex[1]] - textureCoord[triangle[nTriangle].textureIndex[0]]).u;
				deltaV1 = (textureCoord[triangle[nTriangle].textureIndex[1]] - textureCoord[triangle[nTriangle].textureIndex[0]]).v;
				deltaU2 = (textureCoord[triangle[nTriangle].textureIndex[2]] - textureCoord[triangle[nTriangle].textureIndex[1]]).u;
				deltaV2 = (textureCoord[triangle[nTriangle].textureIndex[2]] - textureCoord[triangle[nTriangle].textureIndex[1]]).v;

				// 切线空间各坐标分量
				CVector3 T, B, N;
				CVector3 edge01, edge02;
				edge01 = CVector3(vertex[triangle[nTriangle].vertexIndex[0]], vertex[triangle[nTriangle].vertexIndex[1]]).Normalized() * sqrt(deltaU1 * deltaU1 + deltaV1 * deltaV1);
				edge02 = CVector3(vertex[triangle[nTriangle].vertexIndex[1]], vertex[triangle[nTriangle].vertexIndex[2]]).Normalized() * sqrt(deltaU2 * deltaU2 + deltaV2 * deltaV2);
				T = ((deltaV2 * edge01 - deltaV1 * edge02) / (deltaU1 * deltaV2 - deltaU2 * deltaV1)).Normalized();//理论上可以不做单位化
				B = ((deltaU1 * edge02 - deltaU2 * edge01) / (deltaU1 * deltaV2 - deltaU2 * deltaV1)).Normalized();
				N = triangleNormal;

				TBN[0] = T, TBN[1] = B, TBN[2] = N;
			}

			CP3 point[3];
			for (int j = 0; j < 3; j++)
			{
				point[j] = camera->PerspectiveProjection(vertex[triangle[nTriangle].vertexIndex[j]]);
			}
			// 包围盒
			int Xmax = max(ceil(point[0].x), max(ceil(point[1].x), ceil(point[2].x)));
			int Xmin = min(floor(point[0].x), min(floor(point[1].x), floor(point[2].x)));
			int Ymax = max(ceil(point[0].y), max(ceil(point[1].y), ceil(point[2].y)));
			int Ymin = min(floor(point[0].y), min(floor(point[1].y), floor(point[2].y)));

			for (int x = Xmin; x < Xmax; x++)
			{
				for (int y = Ymin; y < Ymax; y++)
				{
					if ((x + zbuffer->nWidth / 2) < zbuffer->nWidth && (y + zbuffer->nHeight / 2) < zbuffer->nHeight
						&& (x + zbuffer->nWidth / 2) >= 0 && (y + zbuffer->nHeight / 2) >= 0)// zbuffer适应调整
					{
						if (InsideTriangle((float)x + 0.5, (float)y + 0.5, point))
						{
							double Area = point[0].x * (point[1].y - point[2].y) + point[1].x * (point[2].y - point[0].y) + point[2].x * (point[0].y - point[1].y);
							double Area0 = x * (point[1].y - point[2].y) + point[1].x * (point[2].y - y) + point[2].x * (y - point[1].y);
							double Area1 = point[0].x * (y - point[2].y) + x * (point[2].y - point[0].y) + point[2].x * (point[0].y - y);
							double Area2 = point[0].x * (point[1].y - y) + point[1].x * (y - point[0].y) + x * (point[0].y - point[1].y);
							double alpha, beta, gamma;// 重心坐标
							alpha = Area0 / Area, beta = Area1 / Area, gamma = Area2 / Area;
							// 当前点伪深度
							double depth = point[0].z * alpha + point[1].z * beta + point[2].z * gamma;
							if (depth <= zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2])
							{
								zbuffer->depthBuffer[x + zbuffer->nWidth / 2][y + zbuffer->nHeight / 2] = depth;

								// 透视矫正
								double correctAlpha, correctBeta, correctGamma;// 矫正参数
								correctAlpha = alpha / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[0]]).z;
								correctBeta = beta / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[1]]).z;
								correctGamma = gamma / camera->ViewTransform(vertex[triangle[nTriangle].vertexIndex[2]]).z;
								// 当前点的世界空间坐标
								CP3 currentWorldPoint;
								currentWorldPoint = (
									correctAlpha * vertex[triangle[nTriangle].vertexIndex[0]]
									+ correctBeta * vertex[triangle[nTriangle].vertexIndex[1]]
									+ correctGamma * vertex[triangle[nTriangle].vertexIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);
								// 当前点世界空间法向
								CVector3 currentNormal;
								currentNormal = ((
									correctAlpha * normal[triangle[nTriangle].normalIndex[0]]
									+ correctBeta * normal[triangle[nTriangle].normalIndex[1]]
									+ correctGamma * normal[triangle[nTriangle].normalIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma)).Normalized();
								// 当前点的世界空间纹理坐标
								CT2 t = (
									correctAlpha * textureCoord[triangle[nTriangle].textureIndex[0]]
									+ correctBeta * textureCoord[triangle[nTriangle].textureIndex[1]]
									+ correctGamma * textureCoord[triangle[nTriangle].textureIndex[2]]
									) / (correctAlpha + correctBeta + correctGamma);

								if (targetAlbedo != NULL)
								{
									CRGB albedo = targetAlbedo->SampleTextureLF(t);
									material->SetAlbedo(albedo);
									material->UpdateF0();
								}
								if (targetNormal != NULL)
								{
									CRGB sampColor = targetNormal->SampleTextureLF(t);
									sampColor *= 2.0;
									sampColor -= 1.0;
									CVector3 sampNormal;
									sampNormal.x = sampColor.red;
									sampNormal.y = sampColor.green;
									sampNormal.z = sampColor.blue;

									CVector3 T = TBN[0];
									CVector3 N = currentNormal;
									T = (T - DotProduct(T, N) * N).Normalized();
									CVector3 B = CrossProduct(N, T);

									TBN[0] = T, TBN[1] = B, TBN[2] = N;

									currentNormal = CVector3(
										TBN[0].x * sampNormal.x + TBN[1].x * sampNormal.y + TBN[2].x * sampNormal.z,
										TBN[0].y * sampNormal.x + TBN[1].y * sampNormal.y + TBN[2].y * sampNormal.z,
										TBN[0].z * sampNormal.x + TBN[1].z * sampNormal.y + TBN[2].z * sampNormal.z
									).Normalized();
								}
								if (targetMetallic != NULL)
								{
									CRGB metallic = targetMetallic->SampleTextureLF(t);
									material->SetMetallic(metallic.red);
									material->UpdateF0();
								}
								if (targetRoughness != NULL)
								{
									CRGB roughness = targetRoughness->SampleTextureLF(t);
									material->SetRoughness(roughness.red);
									//material->UpdateF0();
								}
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.16) material->metallic = 0.0;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.16 && ((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.5) material->metallic = 0.0;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.5 && ((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.5) material->metallic = 0.5;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.5 && ((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) < 0.80) material->metallic = 0.9;
								//if (((currentWorldPoint.x - scalar * minX) / (scalar * modelLength)) >= 0.80) material->metallic = 0.9;

								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.16) material->roughness = 1.0;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.16 && ((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.38) material->roughness = 0.75;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.38 && ((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.58) material->roughness = 0.5;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.58 && ((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) < 0.80) material->roughness = 0.25;
								//if (((currentWorldPoint.y - scalar * minY) / (scalar * modelHeight)) >= 0.80) material->roughness = 0.05;

								CRGB I = scene->EnvNewPBR(currentWorldPoint, material, currentNormal);
								I.Reinhard();
								pDC->SetPixelV(x, y, CRGBtoRGB(I));
							}
						}
					}
				}
			}
		}
	}
}
