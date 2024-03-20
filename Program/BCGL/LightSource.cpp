#include "pch.h"
#include "LightSource.h"
constexpr auto PI = 3.1415926;

CLightSource::CLightSource(void)
{
	ambientInten = CRGB(0.3, 0.3, 0.3);
	diffuseInten = CRGB(1.0, 1.0, 1.0);
	specularInten = CRGB(1.0, 1.0, 1.0);
	lightColor = CRGB(1.0, 1.0, 1.0);
	lightPosition = CP3(1000, 1000, 1000);
	f = 4 * PI;
	lightOn = TRUE;
}

CLightSource::~CLightSource(void)
{
}

void CLightSource::SetLightPosition(CP3 lightPosition)
{
	this->lightPosition = lightPosition;
}

void CLightSource::SetAmbientInten(CRGB ambientInten)
{
	this->ambientInten = ambientInten;
}

void CLightSource::SetDiffuseInten(CRGB diffuseInten)
{
	this->diffuseInten = diffuseInten;
}

void CLightSource::SetSpecularInten(CRGB specularInten)
{
	this->specularInten = specularInten;
}
