#pragma once

#include "Function3D.h"
#include "Function2D.h"
#include "Triangle2D.h"

class CTracingNode
{
public:
	CTracingNode(void);
	~CTracingNode(void);

private:
	CVector m_dirTracing;
	CPlane  m_planeTracing;
	CTriangle2D m_arrTriangle2D;

	std::vector< CTracingNode* > m_arrTracingNode;
};

