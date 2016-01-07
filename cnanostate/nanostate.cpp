#include "nanostate.h"

#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

#include <msgpack.hpp>

#include <sstream>
#include <vector>

using std::string;
using std::vector;
using std::stringstream;

Nanostate::Nanostate(const char *identity, const char *addr)
{
  _identity = identity;
  _sock = nn_socket(AF_SP, NN_BUS);
  if (( _sock = nn_socket(AF_SP, NN_BUS)) < 0)
    fprintf(stderr, "nn_socket failed: %s\n", nn_strerror(errno));
  if (nn_connect(_sock, addr) < 0)
    fprintf(stderr, "nn_bind failed: %s\n", nn_strerror(errno));

  int rcvtimeo = 50;
  nn_setsockopt(_sock, NN_SOL_SOCKET, NN_RCVTIMEO, &rcvtimeo, sizeof(rcvtimeo));
}

Nanostate::~Nanostate()
{
  if (nn_shutdown(_sock, 0) < 0)
    fprintf(stderr, "nn_shutdown failed: %s\n", nn_strerror(errno));
}

bool Nanostate::send_state_update(const string &name, const vector<char> &data)
{
  stringstream buffer;
  msgpack::type::tuple<string, string, vector<char> > src(_identity, name, data);
  msgpack::pack(buffer, src);

  const string &str = buffer.str();
  int nbytes = nn_send(_sock, str.c_str(), str.length(), 0);
  if (nbytes < 0) // -1
  {
//    fprintf(stderr, "nn_send failed: %s\n", nn_strerror(errno));
    return false;
  }
  return true;
}

bool Nanostate::send_state_update(const string &name, const string &data)
{
  const char *ptr = data.c_str();
  return send_state_update(name, vector<char>(ptr, ptr + data.length()));
}

bool Nanostate::recv_state_update(string &sender, string &name, vector<char> &data)
{
  char *buf = NULL;
  int nbytes = nn_recv(_sock, &buf, NN_MSG, 0);
  if (nbytes < 0) // -1
  {
//    fprintf(stderr, "nn_recv failed: %s\n", nn_strerror(errno));
    return false;
  }
  msgpack::unpacked result;
  msgpack::unpack(result, buf, nbytes);

  msgpack::object deserialized = result.get();
  msgpack::type::tuple<string, string, vector<char> > dst;
  deserialized.convert(&dst);

  sender = dst.get<0>();
  name = dst.get<1>();
  data = dst.get<2>();

  nn_freemsg(buf);
  return true;
}

bool Nanostate::recv_state_update(string &sender, string &name, string &data)
{
  vector<char> vec;
  bool ret = recv_state_update(sender, name, vec);
  data = string(vec.begin(), vec.end());
  return ret;
}

bool Nanostate::has_state_update()
{
  struct nn_pollfd pfd[1];
  pfd[0].fd = _sock;
  pfd[0].events = NN_POLLIN;
  int rc = nn_poll(pfd, 1, 0);
  if (rc == 0)
  {
//    fprintf(stderr, "nn_poll timeout.\n");
    return false;
  }
  if (rc < 0) // -1
  {
//    fprintf(stderr, "nn_poll failed: %s\n", nn_strerror(errno));
    return false;
  }
  return (pfd[0].revents & NN_POLLIN);
}

