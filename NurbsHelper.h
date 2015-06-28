#pragma once

#include "../opennurbs.h"
#include "../examples_linking_pragmas.h"

#include "../example_userdata/example_ud.h"

#include "Function2D.h"
#include "Function3D.h"

#include "RinhoModel.h"
#include "Triangle2D.h"

class NurbsHelper
{
public:
	NurbsHelper(void);
	~NurbsHelper(void);
};

void initModel(ONX_Model &model);
void add_plane(ONX_Model &model, CPlane plane);
void add_line(ONX_Model &model, CPoint point1, CPoint point2);
void add_point(ONX_Model &model, CPoint point);
void add_geoForProjection(ONX_Model &model, CPoint pnt, CPoint pntProjection);

CRinhoModel importModel(const char *sFileName, ON_TextLog& dump);

bool write_points_example( FILE* fp, int version, ON_TextLog& error_log  );
