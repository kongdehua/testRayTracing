#pragma once

#include <vector>

class CFunction2D
{
public:
	CFunction2D(void);
	~CFunction2D(void);
};

class CPoint2D
{
public:
	CPoint2D();
	CPoint2D(double x, double y);

public:
	double X() {return m_lfX;}
	double Y() {return m_lfY;}

private:
	double m_lfX;
	double m_lfY;
};

class CVector2D
{
public:
	CVector2D();
	CVector2D(double x, double y);

private:
	double m_lfX;
	double m_lfY;
};

