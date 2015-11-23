#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main (const int argc, const char **argv)
{
  const char *addr = NULL;
  for (int i = 0; i < argc; i += 1)
  {
    if (strcmp(argv[i], "-b") == 0)
      addr = argv[++i];
  }

  if (addr == NULL)
  {
    fprintf(stderr, "Usage: %s --i IDENTITY -c ADDR\n", argv[0]);
    fprintf(stderr, "ADDR is the address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'\n");
    return 1;
  }

  int sock = nn_socket(AF_SP, NN_BUS);
  assert(sock >= 0);
  assert(nn_bind(sock, addr) >= 0);
  while (1)
  {
    char *buf = NULL;
    int count = nn_recv(sock, &buf, NN_MSG, 0);
    if (count >= 0)
    {
      int send = nn_send(sock, buf, count, 0);
      assert (send == count);
      nn_freemsg (buf);
    }
  }
  return nn_shutdown (sock, 0);
}

