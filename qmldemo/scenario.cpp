#include <QDebug>
#include "scenario.h"

#include "testtorus.h"
#include "femobject.h"




//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>
#include <gmTrianglesystemModule>

// qt
#include <QQuickItem>



void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8, 0.002, 0.0008);
  scene()->insertLight( light, false );

  // Insert Sun
  scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
  front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( front_rcpair.camera.get() );
  front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
  side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( side_rcpair.camera.get() );
  side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Top cam
  auto top_rcpair = createRCPair("Top");
  top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
  top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( top_rcpair.camera.get() );
  top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );







  // Surface visualizers
  auto surface_visualizer = new GMlib::PSurfNormalsVisualizer<float,3>;

  // Surface
  //  auto surface = new TestTorus;
 // surface->toggleDefaultVisualizer();
 // surface->insertVisualizer(surface_visualizer);
 // surface->replot(200,200,1,1);
//  scene()->insert(surface);

 // surface->test01();


  FEMObject *femobj = new FEMObject();
  surface->toggleDefaultVisualizer();
  femobj->makeRegular(4,13,.36f);
  // femobj->triangulateDelaunay();
  //femobj->computeNormals();
  femobj->replot(200,200,1,1);
  femobj->translate(GMlib::Vector<float,3> (8.1,0,0));
  //_scene->insert(facets);
  femobj->computeValue();
  femobj->simulation();
  femobj->stiffness();

}

void Scenario::cleanupScenario() {

}
