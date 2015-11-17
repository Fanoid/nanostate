#include <assert.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

int main (const int argc, const char **argv)
{
  if (argc < 3) 
  {
    fprintf (stderr, "Usage: %s name addr\n", argv[0]);
    return 1;
  }
  const char *name = argv[1];
  const char *addr = argv[2];

  int sock = nn_socket(AF_SP, NN_BUS);
  assert(sock >= 0);
  assert(nn_bind(sock, argv[2]) >= 0);
  while (1)
  {
    char *buf = NULL;
    int count = nn_recv(sock, &buf, NN_MSG, 0);
    if (count >= 0)
    {
      fprintf(stderr, "Received.\n");
      int send = nn_send(sock, buf, count, 0);
      assert (send == count);
      nn_freemsg (buf);
    }
  }
  return nn_shutdown (sock, 0);
}

