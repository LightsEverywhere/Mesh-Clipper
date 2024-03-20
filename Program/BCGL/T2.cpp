#include "pch.h"
#include "T2.h"

CT2::CT2(void)
{
    u = 0.0;
    v = 0.0;
}

CT2::~CT2(void)
{
}

CT2::CT2(double u, double v)
{
    this->u = u;
    this->v = v;
}

CT2 operator+(const CT2& t0, const CT2& t1)
{
    CT2 t;
    t.u = t0.u + t1.u;
    t.v = t0.v + t1.v;
    return t;
}

CT2 operator-(const CT2& t0, const CT2& t1)
{
    CT2 t;
    t.u = t0.u - t1.u;
    t.v = t0.v - t1.v;
    return t;
}

CT2 operator*(const CT2& t, double scalar)
{
    CT2 tCoord;
    tCoord.u = t.u * scalar;
    tCoord.v = t.v * scalar;
    return tCoord;
}

CT2 operator*(double scalar, const CT2& t)
{
    CT2 tCoord;
    tCoord.u = t.u * scalar;
    tCoord.v = t.v * scalar;
    return tCoord;
}

CT2 operator/(const CT2& t, double scalar)
{
    CT2 tCoord;
    tCoord.u = t.u / scalar;
    tCoord.v = t.v / scalar;
    return tCoord;
}

CT2 operator+=(const CT2& t0, const CT2& t1)
{
    return CT2(
        t0.u + t1.u,
        t0.v + t1.v
    );
}

CT2 operator-=(const CT2& t0, const CT2& t1)
{
    return CT2(
        t0.u - t1.u,
        t0.v - t1.v
    );
}

CT2 operator*=(const CT2& t, double scalar)
{
    return CT2(
        t.u * scalar,
        t.v * scalar
    );
}

CT2 operator/=(const CT2& t, double scalar)
{
    return CT2(
        t.u / scalar,
        t.v / scalar
    );
}


