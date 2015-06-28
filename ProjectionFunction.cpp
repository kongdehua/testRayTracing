#include "ProjectionFunction.h"

#include "../opennurbs.h"
#include "../examples_linking_pragmas.h"

#include "../example_userdata/example_ud.h"

#include "Function2D.h"
#include "Function3D.h"

#include "RinhoModel.h"
#include "Triangle2D.h"
#include "NurbsHelper.h"

CProjectionFunction::CProjectionFunction(void)
{
}


CProjectionFunction::~CProjectionFunction(void)
{
}

void convertToInputAxis(CRinhoModel &model, const CPlane &plane)
{
	for (int nIndex = 0; nIndex < model.m_vecPoint.size(); nIndex++)
	{
		model.m_vecPoint[nIndex] = plane.convertPoint(model.m_vecPoint[nIndex]);
	}
}

bool RayTracing()
{
	CRinhoModel model;
	CPlane plane;
	// 三角面片全部转化到入射坐标系
	convertToInputAxis(model, plane);
	// 计算优先级
	
	std::vector<double> vecZMin;
	std::vector<int> vecZIndex;

	int i, j, tempIndex;
	double temp;
	int n = model.m_vecTriangle.size();

	for (i = 0; i < n; i++)
	{
		vecZIndex.push_back(i);
		vecZMin.push_back(min( min(model.m_vecPoint[model.m_vecTriangle[i].first].Z(), 
			                       model.m_vecPoint[model.m_vecTriangle[i].second].Z()),
				                   model.m_vecPoint[model.m_vecTriangle[i].third].Z()));
	}

    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
        {
            if(vecZMin[i] > vecZMin[i + 1])
            {
                temp = vecZMin[i];
                vecZMin[i] = vecZMin[i + 1];
                vecZMin[i + 1] = temp;

                tempIndex = vecZIndex[i];
                vecZIndex[i] = vecZIndex[i + 1];
                vecZIndex[i + 1] = tempIndex;
            }
        }

	std::vector< StuTriangleIndex > vecTriangle;
	for (i = 0; i < n; i++)
	{
		vecTriangle.push_back(model.m_vecTriangle[vecZIndex[i]]);
	}

	// 上面已经按照ZMin重新排序三角面片
	// 接下来就要重新排序

	return true;
}


CPoint calculatorProjectionPoint(CPoint point, CPlane plane)
{
	CPoint pointProjection;
	
	CVector OP( plane.Position() , point);
  CPoint2D po( OP.Dot(plane.XDircection()),
	           OP.Dot(plane.YDircection()));

	//CVector normal1 = po.X() * plane.XDircection();

	//normal1 = po.Y() * plane.YDircection();

	//normal1 = po.X() * plane.XDircection() + po.Y() * plane.YDircection();

	//normal1 = plane.Position() + po.X() * plane.XDircection() + po.Y() * plane.YDircection();

	pointProjection = plane.Position() + po.X() * plane.XDircection() + po.Y() * plane.YDircection();
	return pointProjection;
	//if ();
}


bool projection(FILE* fp, int version, ON_TextLog& error_log)
{
	//CVector dirPlane(0, 0, 1);
	//CPoint pntPlane(0, 0, 0);

	//CPlane plane(pntPlane, dirPlane);
	//CPoint point(100, 55, 100);

	//CVector OP(plane.Position(), point);
	//CPoint2D pnt2D( OP.Dot(plane.X()), OP.Dot(plane.Y()) );

	//CPoint pnt = plane.param(pnt2D);

	//pnt.PrintSelf();
  // example demonstrates how to write a singe points and point clouds
  ONX_Model model;

	//CPoint pntPln(0, 0, 0);
	//CVector normalPln(0.57735, 0, 0.57735);
	//CPlane pln(pntPln, normalPln);
	CPoint pntPlane1(0, 0, 0), pntPlane2(0, 1, 0), pntPlane3(1, 1, 1);
	CPlane pln(pntPlane1, pntPlane2, pntPlane3);
	
	initModel(model);
	add_plane(model, pln);
	add_line(model, pntPlane1, pntPlane2);
	add_line(model, pntPlane1, pntPlane3);
	add_line(model, pntPlane3, pntPlane2);
	
	CPoint pntProjection1(0, 100, 100), pntProjection2(0, 50, 50), pntProjection3(50, 50, 100);
	CPoint pntProjection;
	pntProjection = calculatorProjectionPoint(pntProjection1, pln);
	add_geoForProjection(model, pntProjection1, pntProjection);

	pntProjection = calculatorProjectionPoint(pntProjection2, pln);
	add_geoForProjection(model, pntProjection2, pntProjection);

	pntProjection = calculatorProjectionPoint(pntProjection3, pln);
	add_geoForProjection(model, pntProjection3, pntProjection);

	add_line(model, pntProjection1, pntProjection2);
	add_line(model, pntProjection1, pntProjection3);
	add_line(model, pntProjection3, pntProjection2);



  ON_BinaryFile archive( ON::write3dm, fp ); // fp = pointer from fopoen(...,"wb")
  // start section comment
  const char* sStartSectionComment = __FILE__ "write_points_example()" __DATE__;

  // Set uuid's, indices, etc.
  model.Polish();
  // writes model to archive
  bool ok = model.Write( archive, version, sStartSectionComment, &error_log );

  return ok;

	return true;
}

CPoint2D projection(const CPlane &plane, const CPoint &point)
{
	CVector OP( plane.Position() , point);
  return CPoint2D( OP.Dot(plane.XDircection()),
	           OP.Dot(plane.YDircection()));
}


void projectionModelToPlane(const CRinhoModel model, const CPlane &plane)
{
	for (int i = 0; i < model.m_vecPoint.size(); i++)
	{
		CTriangle2D triFirst, triSecond;
		int nFirst  = model.m_vecTriangle[0].first;
		int nSecond = model.m_vecTriangle[0].second;
		int nThird  = model.m_vecTriangle[0].third;

		triFirst.m_vecPoint.push_back(projection(plane, model.m_vecPoint[nFirst]));
		triFirst.m_vecPoint.push_back(projection(plane, model.m_vecPoint[nSecond]));
		triFirst.m_vecPoint.push_back(projection(plane, model.m_vecPoint[nThird]));

		nFirst  = model.m_vecTriangle[1].first;
		nSecond = model.m_vecTriangle[1].second;
		nThird  = model.m_vecTriangle[1].third;

		triSecond.m_vecPoint.push_back(projection(plane, model.m_vecPoint[nFirst]));
		triSecond.m_vecPoint.push_back(projection(plane, model.m_vecPoint[nSecond]));
		triSecond.m_vecPoint.push_back(projection(plane, model.m_vecPoint[nThird]));
	}
}

