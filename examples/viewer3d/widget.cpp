#include <QMouseEvent>
#include <QDebug>
#include <GL/glut.h>
#include "widget.h"
#include "libqnanostate/qnanostate.h"

using namespace std; 

CGLWidget::CGLWidget(const QGLFormat& fmt, QWidget *parent, QGLWidget *sharedWidget)
  : QGLWidget(fmt, parent, sharedWidget)
{
}

CGLWidget::~CGLWidget()
{
}

void CGLWidget::set_nanostate(QNanostate *nanostate)
{
  _nanostate = nanostate;  
  connect(nanostate, SIGNAL(state_updated(const std::string &, const std::string &, const std::string &)), this, SLOT(update_state(const std::string &,const std::string &,const std::string &)));
}

void CGLWidget::update_state(const string &sender, const string &name, const string &data)
{
  if (sender == _nanostate->get_identity()) return;

  bool updated = false;
  if (name == "trackball") {
    updated |= _trackball.unserialize(data);
  } else if (name == "tf") {
    fprintf(stderr, "tf = %s\n", data.c_str()); 
  } else {
    fprintf(stderr, "identity = %s, name = %s, data = %s\n", sender.c_str(), name.c_str(), data.c_str());
  }

  if (updated) updateGL(); 
}

void CGLWidget::mousePressEvent(QMouseEvent* e)
{
  _trackball.mouse_rotate(e->x(), e->y()); 
}

void CGLWidget::mouseMoveEvent(QMouseEvent* e)
{
  _trackball.motion_rotate(e->x(), e->y()); 

  std::string buf = _trackball.serialize(); 
  _nanostate->send_state_update("trackball", buf); 

  updateGL(); 
}

void CGLWidget::initializeGL()
{
  _trackball.init(); 
}

void CGLWidget::resizeGL(int w, int h)
{
  _trackball.reshape(w, h); 
  glViewport(0, 0, w, h); 
}


void CGLWidget::paintGL()
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glClearColor(0, 0, 0, 0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity(); 
  gluPerspective(45, (float)width()/height(), 0.1, 5); 
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 
  gluLookAt(0, 0, 2.5, 0, 0, 0, 0, 1, 0); 
  
  _trackball.applyTransform(); 

//  glutWireTeapot(0.6);  // This is broken.
  GLUquadric *quad = gluNewQuadric();
  gluCylinder(quad, 0.5, 0.2, 0.7, 50, 25);
}

