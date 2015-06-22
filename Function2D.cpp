#include "Function2D.h"


CFunction2D::CFunction2D(void)
{
}


CFunction2D::~CFunction2D(void)
{
}


CPoint2D::CPoint2D()
	: m_lfX(0)
	, m_lfY(0)
{
}

CPoint2D::CPoint2D(double x, double y)
{
	m_lfX = (x);
	m_lfY = (y);
}

CVector2D::CVector2D()
	: m_lfX(0)
	, m_lfY(0)
{
}

CVector2D::CVector2D(double x, double y)
{
	m_lfX = (x);
	m_lfY = (y);
}
