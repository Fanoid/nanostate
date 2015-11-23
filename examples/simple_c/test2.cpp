#include "cnanostate/nanostate.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main(int argc, const char **argv)
{
  const char *identity = NULL, *addr = NULL;
  for (int i = 0; i < argc; i += 1)
  {
    if (strcmp(argv[i], "-i") == 0)
      identity = argv[++i];
    else if (strcmp(argv[i], "-c") == 0)
      addr = argv[++i];
  }
  if ((identity == "") || (addr == ""))
  {
    fprintf(stderr, "Usage: %s --i IDENTITY -c ADDR\n", argv[0]);
    fprintf(stderr, "IDENTITY is a string to identify this client\n");
    fprintf(stderr, "ADDR is the address of the server, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'\n");
    return 1;
  }

  Nanostate nanostate(identity, addr);
  string sender, name, data;
  sleep(2);
  nanostate.send_state_update("Hello", "World");
  while (true)
  {
    if (nanostate.recv_state_update(sender, name, data) && (sender != identity))
    {
      fprintf(stderr, "%s receive state update from %s: %s: %s\n", identity, sender.c_str(), name.c_str(), data.c_str());
      nanostate.send_state_update("Hello", "World");
    }
  }

  return 1;
}
