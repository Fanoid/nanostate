#include "widget.h"

#include "libqnanostate/qnanostate.h"

#include <stdio.h>
#include <QApplication>
#include <GL/glut.h>

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s <identity> <addr>\n", argv[0]);
    return 1;
  }

  const char *identity = argv[1];
  const char *addr = argv[2];

  fprintf(stderr, "identity = %s, addr = %s\n", identity, addr);

  QApplication app(argc, argv); 
  glutInit(&argc, argv); 

  QNanostate *nanostate = new QNanostate(identity, addr);
  nanostate->start(); 

  CGLWidget *widget = new CGLWidget; 
  widget->show(); 
  widget->set_nanostate(nanostate); 

  return app.exec(); 
}
