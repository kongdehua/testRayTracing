#include "Function3D.h"

#include <math.h>

#include "Function2D.h"

CFunction3D::CFunction3D(void)
{
}


CFunction3D::~CFunction3D(void)
{
}

CPoint::CPoint()
	: m_lfX(0)
	, m_lfY(0)
	, m_lfZ(0)
{
}

CPoint::CPoint(double x, double y, double z)
{
	m_lfX = (x);
	m_lfY = (y);
	m_lfZ = (z);
}

CPoint::CPoint(const CPoint &right)
{
	m_lfX = right.m_lfX;
	m_lfY = right.m_lfY;
	m_lfZ = right.m_lfZ;
}

void CPoint::PrintSelf()
{
	printf("point : %lf, %lf, %lf", m_lfX, m_lfY, m_lfZ);
}

CVector::CVector()
	: m_lfX(0)
	, m_lfY(0)
	, m_lfZ(1)
{
}

CVector::CVector(double x, double y, double z)
{
	SetVector(x, y, z);
	//double D = sqrt(x*x + y*y + z*z);
	//if ( D < 1e-6 )
	//	throw "vector error.";
	//m_lfX = (x);
	//m_lfY = (y);
	//m_lfZ = (z);
}

CVector::CVector(CPoint pnt1)
{
	SetVector(pnt1.X(),
		      pnt1.Y(),
		      pnt1.Z());
}

CVector::CVector(CPoint pnt1, CPoint pnt2)
{
	SetVector(pnt2.X() - pnt1.X(),
		      pnt2.Y() - pnt1.Y(),
		      pnt2.Z() - pnt1.Z());
}

CVector::CVector(const CVector &right)
{
	m_lfX = right.m_lfX;
	m_lfY = right.m_lfY;
	m_lfZ = right.m_lfZ;
}

void CVector::SetVector(double x, double y, double z)
{
	double D = sqrt(x*x + y*y + z*z);
	if ( D < 1e-6 )
		throw "vector SetCoord error.";
	m_lfX = (x);
	m_lfY = (y);
	m_lfZ = (z);
}

CPoint CVector::Added(const CPoint& Other) const
{
	return CPoint(X() + Other.X(), Y() + Other.Y(), Z() + Other.Z());
}

CVector CVector::Added(const CVector& Other) const
{
	return CVector(X() + Other.X(), Y() + Other.Y(), Z() + Other.Z());
}

CVector CVector::Multiplied(const double Scalar) const
{
	return CVector(X()*Scalar, Y()*Scalar, Z()*Scalar);
}

double CVector::Dot(const CVector& Other) const
{
	return X()*Other.X() + Y()*Other.Y() + Z()*Other.Z();
}

CVector CVector::Crossed(const CVector& Right) const
{
	return CVector(Y() * Right.Z() - Z() * Right.Y(),
				   Z() * Right.X() - X() * Right.Z(),
				   X() * Right.Y() - Y() * Right.X());
}

CVector operator* (const double Scalar, const CVector& vector) {
  return vector.Multiplied (Scalar);
}

CPoint operator+ (const CPoint& point, const CVector& vector)
{
	return vector.Added(point);
}

/*****************************************************************************/
CPlane::CPlane()
	: m_pntOrigin(0, 0, 0)
	, m_dirNormal(0, 0, 1)
	, m_dirX(1, 0, 0)
	, m_dirY(0, 1, 0)
{
}

CPlane::CPlane(CPoint origin, CVector normal)
{
	m_pntOrigin = origin;
	m_dirNormal = normal;

	double A = normal.X();
	double B = normal.Y();
	double C = normal.Z();

	double Aabs = A;
	if (Aabs < 0) Aabs = -Aabs;
	double Babs = B;
	if (Babs < 0) Babs = -Babs;
	double Cabs = C;
	if (Cabs < 0) Cabs = -Cabs;

	CVector D;
	if ( Babs < Aabs && Babs <= Cabs )
	{
		if (Aabs > Cabs)   D.SetVector(-C, 0.,  A);
		else               D.SetVector( C, 0., -A);
	}
	else if( Aabs <= Babs && Aabs <= Cabs )
	{
		if (Babs > Cabs)   D.SetVector( 0., -C,  B);
		else               D.SetVector( 0.,  C, -B);
	}
	else {
		if (Aabs > Babs)   D.SetVector( -B,  A, 0.);
		else               D.SetVector(  B, -A, 0.);
	}

	m_dirX = D;
	m_dirY = normal.Crossed(m_dirX);
}

CPlane::CPlane(CPoint pnt1, CPoint pnt2, CPoint pnt3)
{
}

CPoint CPlane::param(CPoint2D pnt2D)
{
	CPoint pnt;
	pnt = this->m_pntOrigin 
		+ pnt2D.X() * this->X()
		+ pnt2D.Y() * this->Y();
	return pnt;
}
