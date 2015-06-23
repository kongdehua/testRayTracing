#include "ProjectionFunction.h"

#include "../opennurbs.h"
#include "../examples_linking_pragmas.h"

#include "../example_userdata/example_ud.h"

#include "Function2D.h"
#include "Function3D.h"

#include "RinhoModel.h"
#include "Triangle2D.h"

CProjectionFunction::CProjectionFunction(void)
{
}


CProjectionFunction::~CProjectionFunction(void)
{
}

CRinhoModel importModel(const char *sFileName, ON_TextLog& dump)
{
	ONX_Model model;

  dump.Print("\nOpenNURBS Archive File:  %ls\n", sFileName );

	ON_wString ws_arg = sFileName;
  const wchar_t* wchar_arg = ws_arg;

  // open file containing opennurbs archive
  FILE* archive_fp = ON::OpenFile( wchar_arg, L"rb");
  if ( !archive_fp ) 
  {
    dump.Print("  Unable to open file.\n" );
    throw "Unable to open file";
  }

  dump.PushIndent();

  // create achive object from file pointer
  ON_BinaryFile archive( ON::read3dm, archive_fp );

  // read the contents of the file into "model"
  bool rc = model.Read( archive, &dump );

  // close the file
  ON::CloseFile( archive_fp );

  // print diagnostic
  if ( rc )
    dump.Print("Successfully read.\n");
  else
    dump.Print("Errors during reading.\n");

  // see if everything is in good shape
  if ( model.IsValid(&dump) )
  {
    dump.Print("Model is valid.\n");
  }
  else
  {
    dump.Print("Model is not valid.\n");
  }

	CRinhoModel rinhoModel;
	for ( int i = 0; i < model.m_object_table.Count(); i++ )
  {
    //dump.Print("Object %d:\n",i);
    //dump.PushIndent();

    // object definition
		if (ON::mesh_object == model.m_object_table[i].m_object->ObjectType())
		{
			model.m_object_table[i].m_object->Dump(dump);

			const ON_Mesh* mesh = dynamic_cast<const ON_Mesh*>(model.m_object_table[i].m_object);
			if ( NULL != mesh )
			{
				dump.Print("find a mesh.\n");
				for ( int i = 0; i < mesh->m_V.Count(); i++)
				{
					CPoint pnt(mesh->m_V[i].x, mesh->m_V[i].y, mesh->m_V[i].z);

					rinhoModel.m_vecPoint.push_back(pnt);
				}
				for ( int i = 0; i < mesh->m_F.Count(); i++)
				{
					StuTriangleIndex tri;
					tri.first = mesh->m_F[i].vi[0];
					tri.second = mesh->m_F[i].vi[1];
					tri.third = mesh->m_F[i].vi[2];
					rinhoModel.m_vecTriangle.push_back(tri);
				}

				dump.Print("mesh face count is %d.\n", rinhoModel.m_vecTriangle.size());
			}
		}

    //dump.PopIndent();
  }
	return  rinhoModel;
	//CRinhoModel model;

}

// init setting , layers, groups
void initModel(ONX_Model &model)
{
	// file properties (notes, preview image, revision history, ...)
  {
    // set revision history information
    model.m_properties.m_RevisionHistory.NewRevision();
    
    // set application information
    model.m_properties.m_Application.m_application_name = "OpenNURBS write_points_example() function";
    model.m_properties.m_Application.m_application_URL = "http://www.opennurbs.org";
    model.m_properties.m_Application.m_application_details = "Example program in OpenNURBS toolkit.";

    {
      // OPTIONAL - add some notes
      model.m_properties.m_Notes.m_notes = "This file was made with the OpenNURBS write_points_example() function.";
      model.m_properties.m_Notes.m_bVisible = false;
    }
  }

  // file settings (units, tolerances, views, ...)
  {
    // OPTIONAL - change values from defaults
    model.m_settings.m_ModelUnitsAndTolerances.m_unit_system = ON::meters;
    model.m_settings.m_ModelUnitsAndTolerances.m_absolute_tolerance = 0.01;
    model.m_settings.m_ModelUnitsAndTolerances.m_angle_tolerance = ON_PI/180.0; // radians
    model.m_settings.m_ModelUnitsAndTolerances.m_relative_tolerance = 0.01; // 1%
  }


	// layer table
  {
    // define some layers
    ON_Layer layer[3];

    layer[0].SetLayerName("Default");
    layer[0].SetVisible(true);
    layer[0].SetLocked(false);
    layer[0].SetLayerIndex(0);
    layer[0].SetColor( ON_Color(0,0,0) );

    layer[1].SetLayerName("red points");
    layer[1].SetVisible(true);
    layer[1].SetLocked(false);
    layer[1].SetLayerIndex(1);
    layer[1].SetColor( ON_Color(255,0,0) );

    layer[2].SetLayerName("one blue point");
    layer[2].SetVisible(true);
    layer[2].SetLocked(false);
    layer[2].SetLayerIndex(2);
    layer[2].SetColor( ON_Color(0,0,255) );

    model.m_layer_table.Append(layer[0]);
    model.m_layer_table.Append(layer[1]);
    model.m_layer_table.Append(layer[2]);
  }

  // group table
  {
    // we'll put 2 red and one blue point in a group
    ON_Group group;
    group.SetGroupName("group of points");
    group.SetGroupIndex(0);
    model.m_group_table.Append(group);

		group.SetGroupName("group1 of points");
    group.SetGroupIndex(1);
    model.m_group_table.Append(group);
  }

}

void add_plane(ONX_Model &model, CPlane plane)
{
	// 绘制面
	bool bHasVertexNormals = false; // we will specify vertex normals
  bool bHasTexCoords = false;    // we will not specify texture coordinates
  const int vertex_count = 3;  // 4 duplicates for different base normals
  const int face_count = 1; // 4 triangle sides and a quad base

	ON_Mesh *mesh = new ON_Mesh( face_count, vertex_count, bHasVertexNormals, bHasTexCoords);
  // The SetVertex(), SetNormal(), SetTCoord() and SetFace() functions
  // return true if successful and false if input is illegal.  It is
  // a good idea to inspect this returned value.

	double lfWidth = 100, lfHeight = 100;
	double x = 0, y = 0, z = 0;
	x = plane.Position().X() - lfWidth * plane.XDircection().X() - lfHeight * plane.YDircection().X();
	y = plane.Position().Y() - lfWidth * plane.XDircection().Y() - lfHeight * plane.YDircection().Y();
	z = plane.Position().Z() - lfWidth * plane.XDircection().Z() - lfHeight * plane.YDircection().Z();
  // vertex #0: apex location and normal
  mesh->SetVertex( 0, ON_3dPoint(	x, y, z) );  //mesh.SetVertexNormal( 0, ON_3dVector(0.0,  0.0,  1.0) );

	x = plane.Position().X() + lfWidth * plane.XDircection().X() - lfHeight * plane.YDircection().X();
	y = plane.Position().Y() + lfWidth * plane.XDircection().Y() - lfHeight * plane.YDircection().Y();
	z = plane.Position().Z() + lfWidth * plane.XDircection().Z() - lfHeight * plane.YDircection().Z();
  // vertex #1: SW corner vertex for sides
  mesh->SetVertex( 1, ON_3dPoint( x, y, z) );  //mesh.SetVertexNormal( 1, ON_3dVector(-1.0, -1.0,  0.0) ); // set normal will unitize if needed

	x = plane.Position().X() + lfWidth * plane.XDircection().X() + lfHeight * plane.YDircection().X();
	y = plane.Position().Y() + lfWidth * plane.XDircection().Y() + lfHeight * plane.YDircection().Y();
	z = plane.Position().Z() + lfWidth * plane.XDircection().Z() + lfHeight * plane.YDircection().Z();
  // vertex #2: SE corner vertex for sides
  mesh->SetVertex( 2, ON_3dPoint( x, y, z) );  //mesh.SetVertexNormal( 2, ON_3dVector(+1.0, -1.0,  0.0) );

	x = plane.Position().X() - lfWidth * plane.XDircection().X() + lfHeight * plane.YDircection().X();
	y = plane.Position().Y() - lfWidth * plane.XDircection().Y() + lfHeight * plane.YDircection().Y();
	z = plane.Position().Z() - lfWidth * plane.XDircection().Z() + lfHeight * plane.YDircection().Z();
  // vertex #3: SE corner vertex for sides
  mesh->SetVertex( 3, ON_3dPoint( x, y, z) );  //mesh.SetVertexNormal( 2, ON_3dVector(+1.0, -1.0,  0.0) );

	// South side triangle
	mesh->SetQuad( 0,   0, 1, 2, 3 );
	{
    ONX_Model_Object& mo = model.m_object_table.AppendNew();
    mo.m_object = mesh;
    mo.m_bDeleteObject = false; // point1 is on the stack
    mo.m_attributes.m_layer_index = 0;
//    mo.m_attributes.AddToGroup(1); // put this points in the group
    mo.m_attributes.m_name = "first mesh";
  }

	// 绘制交叉线
}

void add_line(ONX_Model &model, CPoint point1, CPoint point2)
{

	ON_LineCurve *line = new ON_LineCurve( ON_Line( 
							ON_3dPoint(point1.X(), point1.Y(), point1.Z()), 
							ON_3dPoint(point2.X(), point2.Y(), point2.Z()) ) );
	{
    // write a line on the default layer
    ONX_Model_Object& mo = model.m_object_table.AppendNew();
    mo.m_object = line;
    mo.m_bDeleteObject = true;
    mo.m_attributes.m_layer_index = 0;
    mo.m_attributes.m_name = "straight line curve";
  }
}

void add_point(ONX_Model &model, CPoint point)
{
  // single point at (1,4,5) on default layer
	ON_Point *point1 = new ON_Point(ON_3dPoint( point.X(), point.Y(), point.Z() ));
  point1->AttachUserData( new CExampleWriteUserData("write_points_example()-point1") );
  {
    ONX_Model_Object& mo = model.m_object_table.AppendNew();
    mo.m_object = point1;
    mo.m_bDeleteObject = false; // point1 is on the stack
    mo.m_attributes.m_layer_index = 0;
    mo.m_attributes.AddToGroup(1); // put this points in the group
    mo.m_attributes.m_name = "first point";
  }

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

void add_geoForProjection(ONX_Model &model, CPoint pnt, CPoint pntProjection)
{
	add_line(model, pnt, pntProjection);
	add_point(model, pnt);
	add_point(model, pntProjection);
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

bool write_points_example( FILE* fp, int version, ON_TextLog& error_log  )
{
  // example demonstrates how to write a singe points and point clouds
  ONX_Model model;


  // file properties (notes, preview image, revision history, ...)
  {
    // set revision history information
    model.m_properties.m_RevisionHistory.NewRevision();
    
    // set application information
    model.m_properties.m_Application.m_application_name = "OpenNURBS write_points_example() function";
    model.m_properties.m_Application.m_application_URL = "http://www.opennurbs.org";
    model.m_properties.m_Application.m_application_details = "Example program in OpenNURBS toolkit.";

    {
      // OPTIONAL - add some notes
      model.m_properties.m_Notes.m_notes = "This file was made with the OpenNURBS write_points_example() function.";
      model.m_properties.m_Notes.m_bVisible = true;
    }
  }

  // file settings (units, tolerances, views, ...)
  {
    // OPTIONAL - change values from defaults
    model.m_settings.m_ModelUnitsAndTolerances.m_unit_system = ON::meters;
    model.m_settings.m_ModelUnitsAndTolerances.m_absolute_tolerance = 0.01;
    model.m_settings.m_ModelUnitsAndTolerances.m_angle_tolerance = ON_PI/180.0; // radians
    model.m_settings.m_ModelUnitsAndTolerances.m_relative_tolerance = 0.01; // 1%
  }

  // layer table
  {
    // define some layers
    ON_Layer layer[3];

    layer[0].SetLayerName("Default");
    layer[0].SetVisible(true);
    layer[0].SetLocked(false);
    layer[0].SetLayerIndex(0);
    layer[0].SetColor( ON_Color(0,0,0) );

    layer[1].SetLayerName("red points");
    layer[1].SetVisible(true);
    layer[1].SetLocked(false);
    layer[1].SetLayerIndex(1);
    layer[1].SetColor( ON_Color(255,0,0) );

    layer[2].SetLayerName("one blue point");
    layer[2].SetVisible(true);
    layer[2].SetLocked(false);
    layer[2].SetLayerIndex(2);
    layer[2].SetColor( ON_Color(0,0,255) );

    model.m_layer_table.Append(layer[0]);
    model.m_layer_table.Append(layer[1]);
    model.m_layer_table.Append(layer[2]);
  }

  // group table
  {
    // we'll put 2 red and one blue point in a group
    ON_Group group;
    group.SetGroupName("group of points");
    group.SetGroupIndex(0);
    model.m_group_table.Append(group);
  }

  // object table

  // single point at (1,4,5) on default layer
  ON_Point point1(ON_3dPoint( 1.0, 4.0, 5.0 ));
  point1.AttachUserData( new CExampleWriteUserData("write_points_example()-point1") );
  {
    ONX_Model_Object& mo = model.m_object_table.AppendNew();
    mo.m_object = &point1;
    mo.m_bDeleteObject = false; // point1 is on the stack
    mo.m_attributes.m_layer_index = 0;
    mo.m_attributes.m_name = "first point";
  }

  // point "cloud" with 3 points on red point cloud layer
  ON_PointCloud* pointcloud = new ON_PointCloud();
  pointcloud->AppendPoint(ON_3dPoint( 1.0, 6.0, 5.0 ));
  pointcloud->AppendPoint(ON_3dPoint( 1.5, 4.5, 6.0 ));
  pointcloud->AppendPoint(ON_3dPoint( 2.0, 5.0, 7.0 ));

  pointcloud->AttachUserData( new CExampleWriteUserData("write_points_example()-pointcloud") );
  {
    ONX_Model_Object& mo = model.m_object_table.AppendNew();
    mo.m_object = pointcloud;
    mo.m_bDeleteObject = true; // ~ONX_Model will delete pointcloud.
    mo.m_attributes.m_layer_index = 1;
    mo.m_attributes.AddToGroup(0); // put these points in the group
    mo.m_attributes.m_name = "3 points";
  }

  // single point at (3,2,4) on red point layer
  ON_Point point2(ON_3dPoint( 3.0, 2.0, 4.0  ));
  point2.AttachUserData( new CExampleWriteUserData("write_points_example()-point2") );
  {
    ONX_Model_Object& mo = model.m_object_table.AppendNew();
    mo.m_object = &point2;
    mo.m_bDeleteObject = false;
    mo.m_attributes.m_layer_index = 2;
    mo.m_attributes.AddToGroup(0); // put this points in the group
    mo.m_attributes.m_name = "last point";
  }

  ON_BinaryFile archive( ON::write3dm, fp ); // fp = pointer from fopoen(...,"wb")

  // start section comment
  const char* sStartSectionComment = __FILE__ "write_points_example()" __DATE__;

  // Set uuid's, indices, etc.
  model.Polish();
  // writes model to archive
  bool ok = model.Write( archive, version, sStartSectionComment, &error_log );

  return ok;
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