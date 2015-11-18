#include "qnanostate.h"

#include <stdio.h>

using std::string;

QNanostate::QNanostate(const char *identity, const char *addr)
  : Nanostate(identity, addr), _interval(10)
{
}

QNanostate::~QNanostate()
{
}

void QNanostate::run()
{
  while (true)
  {
    msleep(_interval);
    if (has_state_update())
    {
      printf("state updated\n");  // Without this, the program will stuck. TODO
      emit state_updated();
    }
  }
}

