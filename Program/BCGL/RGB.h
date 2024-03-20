#pragma once
#include"../framework.h"

class CRGB
{
public:
	CRGB(void);
	virtual ~CRGB(void);
	CRGB(double red, double green, double blue);
	void Normalize(void);// 归一化到[0，1]区间
	void Reinhard();// Reinhard色调映射(包含Gamma校正)
	void Exposure(float exposure);// 曝光色调映射(包含Gamma校正)
	// 运算符重载
	friend CRGB operator+(const CRGB& c0, const CRGB& c1);
	friend CRGB operator+(const CRGB& c, double num);
	friend CRGB operator+(double num, const CRGB& c);
	friend CRGB operator-(const CRGB& c0, const CRGB& c1);
	friend CRGB operator-(double num, const CRGB& c);
	friend CRGB operator-(const CRGB& c);
	friend CRGB operator*(const CRGB& c0, const CRGB& c1);
	friend CRGB operator*(double scalar, const CRGB& c);
	friend CRGB operator*(const CRGB& c, double scalar);
	friend CRGB operator/(const CRGB& c, double scalar);
	friend CRGB operator/(const CRGB& c0, const CRGB& c1);
	friend CRGB operator+=(CRGB& c0, CRGB& c1);
	friend CRGB operator-=(CRGB& c0, CRGB& c1);
	friend CRGB operator-=(CRGB& c, double num);
	friend CRGB operator*=(CRGB& c0, CRGB& c1);
	friend CRGB operator*=(CRGB& c, double scalar);
	friend CRGB operator/=(CRGB& c, double scalar);
	friend CRGB pow(const CRGB& c, double exponent);
	friend CRGB exp(const CRGB& c);
	friend CRGB mix(const CRGB& c0, const CRGB& c1, float a);// 线性混叠函数，返回 c0*(1-a) + c1*a

public:
	double red, green, blue;
};

