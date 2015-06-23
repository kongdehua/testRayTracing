#pragma once

#include <vector>
#include "Function3D.h"
//typedef struct _StuPoint
//{
//	double x;
//	double y;
//	double z;
//} StuPoint;

typedef struct _StuTriangleIndex
{
	int first;
	int second;
	int third;
} StuTriangleIndex;

class CRinhoModel
{
public:
	CRinhoModel(void);
	~CRinhoModel(void);

	//std::vector< StuTriangleIndex > GetTriangles() const;

public:
	std::vector< StuTriangleIndex > m_vecTriangle;
	std::vector< CPoint > m_vecPoint;
};

class CProjectModel
{
public:
	CProjectModel(void);
	~CProjectModel(void);

	//std::vector< StuTriangleIndex > GetTriangles() const;

public:
	std::vector< StuTriangleIndex > m_vecTriangle;
	std::vector< CPoint2D > m_vecPoint2D;
};
