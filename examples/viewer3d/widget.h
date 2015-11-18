#ifndef _WIDGET_H
#define _WIDGET_H

#include <QGLWidget>
#include <QList>
#include "trackball.h"

class QNanostate;
class QMouseEvent; 

class CGLWidget : public QGLWidget
{
  Q_OBJECT 
public:
  CGLWidget(const QGLFormat& fmt=QGLFormat::defaultFormat(), QWidget *parent=NULL, QGLWidget *sharedWidget=NULL); 
  ~CGLWidget(); 

  void set_nanostate(QNanostate *nanostate); 

protected slots: 
  void update_state(const std::string &, const std::string &, const std::string &);

protected:
  void initializeGL(); 
  void resizeGL(int w, int h); 
  void paintGL(); 

  void mousePressEvent(QMouseEvent*); 
  void mouseMoveEvent(QMouseEvent*);

  void parseDomainSize(); 

private: 
  CGLTrackball _trackball; 
  QNanostate *_nanostate; 
}; 

#endif
