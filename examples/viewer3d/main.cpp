#include "widget.h"

#include "qtnanostate/qnanostate.h"

#include <stdio.h>
#include <QApplication>
#include <GL/glut.h>

int main(int argc, char **argv)
{
  const char *identity = NULL, *addr = NULL;
  for (int i = 0; i < argc; i += 1)
  {
    if (strcmp(argv[i], "-i") == 0)
      identity = argv[++i];
    else if (strcmp(argv[i], "-c") == 0)
      addr = argv[++i];
  }
  if ((identity == NULL) || (addr == NULL))
  {
    fprintf(stderr, "Usage: %s --i IDENTITY -c ADDR\n", argv[0]);
    fprintf(stderr, "IDENTITY is a string to identify this client\n");
    fprintf(stderr, "ADDR is the address of the server, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'\n");
    return 1;
  }

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
