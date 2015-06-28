#pragma once

#include "../opennurbs.h"

#include "RinhoModel.h"
#include "Function2D.h"
#include "Function3D.h"

class CProjectionFunction
{
public:
	CProjectionFunction(void);
	~CProjectionFunction(void);
};

bool write_points_example( FILE* fp, int version, ON_TextLog& error_log  );

bool RayTracing();

CRinhoModel importModel(const char *sFileName, ON_TextLog& dump);
bool projection(FILE* fp, int version, ON_TextLog& error_log);

CPoint2D projection(const CPlane &plane, const CPoint &point);
void projectionModelToPlane(const CRinhoModel model, const CPlane &plane);