#include "pch.h"
#include "RGB.h"

CRGB::CRGB(void)
{
	red = 0.0;
	green = 0.0;
	blue = 0.0;
}

CRGB::~CRGB(void)
{
}

CRGB::CRGB(double red, double green, double blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void CRGB::Normalize(void)
{
	red = red < 0.0 ? 0.0 : (red > 1.0 ? 1.0 : red);
	green = green < 0.0 ? 0.0 : (green > 1.0 ? 1.0 : green);
	blue = blue < 0.0 ? 0.0 : (blue > 1.0 ? 1.0 : blue);
}

void CRGB::Reinhard()
{
	const float gamma = 2.2;// 显示器gamma
	CRGB hdrColor(this->red, this->green, this->blue);
	// Reinhard色调映射
	CRGB mapped = hdrColor / (hdrColor + CRGB(1.0, 1.0, 1.0));
	// Gamma校正
	mapped = pow(mapped, (1.0 / gamma));

	this->red = mapped.red;
	this->green = mapped.green;
	this->blue = mapped.blue;
}

void CRGB::Exposure(float exposure)
{
	const float gamma = 2.2;// 显示器gamma
	CRGB hdrColor(this->red, this->green, this->blue);
	// 曝光色调映射
	CRGB mapped = CRGB(1.0, 1.0, 1.0) - exp(-hdrColor * exposure);
	// Gamma校正
	mapped = pow(mapped, (1.0 / gamma));

	this->red = mapped.red;
	this->green = mapped.green;
	this->blue = mapped.blue;
}

CRGB operator+(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red + c1.red;
	color.green = c0.green + c1.green;
	color.blue = c0.blue + c1.blue;
	return color;
}

CRGB operator+(const CRGB& c, double num)
{
	CRGB color;
	color.red = num + c.red;
	color.green = num + c.green;
	color.blue = num + c.blue;
	return color;
}

CRGB operator+(double num, const CRGB& c)
{
	CRGB color;
	color.red = num + c.red;
	color.green = num + c.green;
	color.blue = num + c.blue;
	return color;
}

CRGB operator-(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red - c1.red;
	color.green = c0.green - c1.green;
	color.blue = c0.blue - c1.blue;
	return color;
}

CRGB operator-(double num, const CRGB& c)
{
	CRGB color;
	color.red = num - c.red;
	color.green = num - c.green;
	color.blue = num - c.blue;
	return color;
}

CRGB operator-(const CRGB& c)
{
	CRGB color;
	color.red = -c.red;
	color.green = -c.green;
	color.blue = -c.blue;
	return color;
}

CRGB operator*(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red * c1.red;
	color.green = c0.green * c1.green;
	color.blue = c0.blue * c1.blue;
	return color;
}

CRGB operator*(double scalar, const CRGB& c)
{
	CRGB color;
	color.red = c.red * scalar;
	color.green = c.green * scalar;
	color.blue = c.blue * scalar;
	return color;
}

CRGB operator*(const CRGB& c, double scalar)
{
	CRGB color;
	color.red = c.red * scalar;
	color.green = c.green * scalar;
	color.blue = c.blue * scalar;
	return color;
}

CRGB operator/(const CRGB& c, double scalar)
{
	CRGB color;
	color.red = c.red / scalar;
	color.green = c.green / scalar;
	color.blue = c.blue / scalar;
	return color;
}

CRGB operator/(const CRGB& c0, const CRGB& c1)
{
	CRGB color;
	color.red = c0.red / c1.red;
	color.green = c0.green / c1.green;
	color.blue = c0.blue / c1.blue;
	return color;
}

CRGB operator+=(CRGB& c0, CRGB& c1)
{
	c0.red += c1.red;
	c0.green += c1.green;
	c0.blue += c1.blue;
	return c0;
}

CRGB operator-=(CRGB& c0, CRGB& c1)
{
	c0.red -= c1.red;
	c0.green -= c1.green;
	c0.blue -= c1.blue;
	return c0;
}

CRGB operator-=(CRGB& c, double num)
{
	c.red -= num;
	c.green -= num;
	c.blue -= num;
	return c;
}

CRGB operator*=(CRGB& c0, CRGB& c1)
{
	c0.red *= c1.red;
	c0.green *= c1.green;
	c0.blue *= c1.blue;
	return c0;
}

CRGB operator*=(CRGB& c, double scalar)
{
	c.red *= scalar;
	c.green *= scalar;
	c.blue *= scalar;
	return c;
}

CRGB operator/=(CRGB& c, double scalar)
{
	c.red /= scalar;
	c.green /= scalar;
	c.blue /= scalar;
	return c;
}

CRGB pow(const CRGB& c, double exponent)
{
	CRGB color;
	color.red = pow(c.red, exponent);
	color.green = pow(c.green, exponent);
	color.blue = pow(c.blue, exponent);
	return color;
}

CRGB exp(const CRGB& c)
{
	CRGB color;
	color.red = exp(c.red);
	color.green = exp(c.green);
	color.blue = exp(c.blue);
	return color;
}

CRGB mix(const CRGB& c0, const CRGB& c1, float a)
{
	return c0 * (1.0 - a) + c1 * a;// 线性混叠
}
