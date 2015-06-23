#pragma once

#include "Function2D.h"

class CFunction3D
{
public:
	CFunction3D(void);
	~CFunction3D(void);
};

class CVector;

class CPoint
{
public:
	CPoint();
	CPoint(double x, double y, double z);

	CPoint(const CPoint &right);

	void PrintSelf();
public:
	double X() const {return m_lfX;}
	double Y() const {return m_lfY;}
	double Z() const {return m_lfZ;}

public:
	void SetCoord(double x, double y, double z);

private:
	double m_lfX;
	double m_lfY;
	double m_lfZ;
};

class CVector
{
public:
	CVector();
	CVector(double x, double y, double z);
	CVector(const CPoint &pnt1);
	CVector(const CPoint &pnt1, const CPoint &Pnt2);

	CVector(const CVector &right);

	void PrintSelf();
public:
	double X() const {return m_lfX;}
	double Y() const {return m_lfY;}
	double Z() const {return m_lfZ;}

public:
	void SetVector(double x, double y, double z);

public:
	double Modulus() const;

public:
	void Normalize() ;
	CVector Normalized () const;

public:
	CPoint Added(const CPoint& Other) const;
	CPoint operator +(const CPoint& Other) const
	{
	  return Added(Other);
	}

	CVector Added(const CVector& Other) const;
	CVector operator +(const CVector& Other) const
	{
	  return Added(Other);
	}

	CVector Multiplied(const double Scalar) const;
	CVector operator *(const double Scalar) const
	{
	  return Multiplied(Scalar);
	}

	//! computes the scalar product between <me> and Other <br>
	double Dot(const CVector& Other) const;
	double operator *(const CVector& Other) const
	{
	  return Dot(Other);
	}

    CVector Crossed(const CVector& Right) const;
    CVector operator ^(const CVector& Right) const
	{
	  return Crossed(Right);
	}

private:
	double m_lfX;
	double m_lfY;
	double m_lfZ;
};

inline CVector operator* (const double Scalar, const CVector& vector);
inline CPoint operator+ (const CPoint& point, const CVector& vector);

class CPlane
{
public:
	CPlane();
	CPlane(CPoint origin, CVector normal);
	CPlane(CPoint pnt1, CPoint pnt2, CPoint pnt3);

public:
	CPoint  Position()     const { return m_pntOrigin; }
	CVector Normal()       const { return m_dirNormal; }
	CVector XDircection()  const { return m_dirX;      }
	CVector YDircection()  const { return m_dirY;      }

public:
	CPoint param(CPoint2D pnt2D);

private:
	CPoint m_pntOrigin;
	CVector m_dirNormal;
	CVector m_dirX;
	CVector m_dirY;
};
