#pragma once

#include "../opennurbs.h"

class CProjectionFunction
{
public:
	CProjectionFunction(void);
	~CProjectionFunction(void);
};

bool write_points_example( FILE* fp, int version, ON_TextLog& error_log  );

bool projection();
