#ifndef _NANOSTATE_H_
#define _NANOSTATE_H_

#include <string>

class Nanostate
{
public:
  explicit Nanostate(const char *identity, const char *addr);
  ~Nanostate();
public:
  bool send_state_update(const std::string &name, const std::string &data);
  bool recv_state_update(std::string &sender, std::string &name, std::string &data);
  bool has_state_update();
  const std::string &get_identity() const { return _identity; }
protected:
  std::string _identity;
  int _sock;
};

#endif  // _NANOSTATE_H_
