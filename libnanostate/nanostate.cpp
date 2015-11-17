#include "nanostate.h"

#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

#include <msgpack.hpp>

#include <sstream>

using std::string;
using std::stringstream;

Nanostate::Nanostate(const char *identity, const char *addr)
{
  _identity = identity;
  _sock = nn_socket(AF_SP, NN_BUS);
  if (( _sock = nn_socket(AF_SP, NN_BUS)) < 0)
    fprintf(stderr, "nn_socket %s\n", nn_strerror(errno));
  if (nn_connect(_sock, addr) < 0)
    fprintf(stderr, "nn_bind %s\n", nn_strerror(errno));
}

Nanostate::~Nanostate()
{
  if (nn_shutdown(_sock, 0) < 0)
    fprintf(stderr, "nn_shutdown %s\n", nn_strerror(errno));
}

void Nanostate::send_state_update(const string &name, const string &data)
{
  msgpack::type::tuple<string, string, string> src(_identity, name, data);
  stringstream buffer;
  msgpack::pack(buffer, src);

  const string &str = buffer.str();
  int nbytes = nn_send(_sock, str.c_str(), str.length(), 0);
  if (nbytes < 0)
    fprintf(stderr, "nn_send %s\n", nn_strerror(errno));
}

void Nanostate::recv_state_update(string &sender, string &name, string &data)
{
  char *buf = NULL;
  int nbytes = nn_recv(_sock, &buf, NN_MSG, 0);
  if (nbytes > 0)
  {
    msgpack::unpacked result;
    msgpack::unpack(result, buf, nbytes);

    msgpack::object deserialized = result.get();
    msgpack::type::tuple<string, string, string> dst;
    deserialized.convert(&dst);

    sender = dst.get<0>();
    name = dst.get<1>();
    data = dst.get<2>();
  }
  nn_freemsg(buf);
}

bool Nanostate::has_state_update()
{
  struct nn_pollfd pfd;
  pfd.fd = _sock;
  pfd.events = NN_POLLIN;
  int rc = nn_poll(&pfd, 1, 1000);
  if (rc < 0)
  {
    fprintf(stderr, "nn_poll %s\n", nn_strerror(errno));
    return false;
  }
  return rc > 0;
}

