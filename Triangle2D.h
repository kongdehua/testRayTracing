#pragma once
#include "Function2D.h"

class CTriangle2D
{
public:
	CTriangle2D(void);
	~CTriangle2D(void);
public:
	std::vector< CPoint2D > m_vecPoint;
};