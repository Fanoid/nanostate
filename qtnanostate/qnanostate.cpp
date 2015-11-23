#include "qnanostate.h"

#include <stdio.h>

using std::string;

QNanostate::QNanostate(const char *identity, const char *addr)
  : Nanostate(identity, addr), _interval(10)
{
  qRegisterMetaType<std::string>("std::string");
}

QNanostate::~QNanostate()
{
}

void QNanostate::run()
{
  string sender, name, data;
  while (true)
  {
    msleep(_interval);
    if (recv_state_update(sender, name, data))
    {
      emit state_updated(sender, name, data);
    }
  }
}

