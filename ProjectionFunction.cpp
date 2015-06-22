#include "ProjectionFunction.h"

#include "../opennurbs.h"
#include "../examples_linking_pragmas.h"

#include "../example_userdata/example_ud.h"

#include "Function2D.h"
#include "Function3D.h"

CProjectionFunction::CProjectionFunction(void)
{
}


CProjectionFunction::~CProjectionFunction(void)
{
}

bool projection()
{
	CVector dirPlane(0, 0, 1);
	CPoint pntPlane(0, 0, 0);

	CPlane plane(pntPlane, dirPlane);
	CPoint point(100, 55, 100);

	CVector OP(plane.Position(), point);
	CPoint2D pnt2D( OP.Dot(plane.X()), OP.Dot(plane.Y()) );

	CPoint pnt = plane.param(pnt2D);

	pnt.PrintSelf();
	return true;
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
