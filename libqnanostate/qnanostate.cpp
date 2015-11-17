#include "qnanostate.h"

#include <stdio.h>

using std::string;

QNanostate::QNanostate(const char *identity, const char *addr)
  : Nanostate(identity, addr), _interval(20000)
{
}

QNanostate::~QNanostate()
{
}

void QNanostate::run()
{
  while (true)
  {
    usleep(_interval / 2);
    if (has_state_update())
    {
      emit state_updated();
    }
    usleep(_interval / 2);
  }
}

