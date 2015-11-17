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
  connect(nanostate, SIGNAL(state_updated()), this, SLOT(update_state()));
}

void CGLWidget::update_state()
{
  bool updated = false;

  string sender, name, data;
  _nanostate->recv_state_update(sender, name, data);
  if (sender == _nanostate->get_identity()) return;

  if (name == "trackball") {
    updated |= _trackball.unserialize(data);
  } else if (name == "tf") {
    fprintf(stderr, "tf = %s\n", data.c_str()); 
  }

  if (updated) 
    updateGL(); 
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
  glClearColor(0, 0, 0, 0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity(); 
  gluPerspective(45, (float)width()/height(), 0.1, 5); 
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 
  gluLookAt(0, 0, 2.5, 0, 0, 0, 0, 1, 0); 
  
  _trackball.applyTransform(); 

  glColor3f(1, 1, 1); 
  glBegin(GL_LINE_STRIP);
  glVertex3f(0.1, 0.1, 0.1);
  glVertex3f(0.1, 0.1, 0.9);
  glVertex3f(0.1, 0.9, 0.9);
  glVertex3f(0.1, 0.9, 0.1);
  glVertex3f(0.9, 0.9, 0.1);
  glVertex3f(0.9, 0.9, 0.9);
  glVertex3f(0.9, 0.1, 0.9);
  glVertex3f(0.9, 0.1, 0.1);
  glEnd();
//  glutWireTeapot(0.6); 
}

