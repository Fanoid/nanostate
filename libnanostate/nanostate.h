#ifndef _NANOSTATE_H_
#define _NANOSTATE_H_

#include <string>

class Nanostate
{
public:
  explicit Nanostate(const char *identity, const char *addr);
  ~Nanostate();
public:
  void send_state_update(const std::string &name, const std::string &data);
  void recv_state_update(std::string &sender, std::string &name, std::string &data);
  bool has_state_update();
private:
  std::string _identity;
  int _sock;
};

#endif  // _NANOSTATE_H_
