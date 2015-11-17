#include "libnanostate/nanostate.h"

#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, const char **argv)
{
  Nanostate nanostate(argv[1], argv[2]);
  sleep(1);
  while (true)
  {
    nanostate.send_state_update("Hello", "World");
    sleep(1);
  }

  return 1;
}
