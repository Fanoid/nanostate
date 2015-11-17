#include "libnanostate/nanostate.h"

#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, const char **argv)
{
  Nanostate nanostate(argv[1], argv[2]);
  string sender, name, data;
  while (true)
  {
    nanostate.recv_state_update(sender, name, data);
    fprintf(stderr, "recv_state: %s: %s: %s\n", sender.c_str(), name.c_str(), data.c_str());
  }

  return 1;
}
