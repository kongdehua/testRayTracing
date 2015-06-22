#pragma once
#include "Function2D.h"

class CTriangle2D
{
public:
	CTriangle2D(void);
	~CTriangle2D(void);
private:
	std::vector< CPoint2D > m_vecPoint;
};