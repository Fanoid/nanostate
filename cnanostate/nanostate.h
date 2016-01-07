#ifndef _NANOSTATE_H_
#define _NANOSTATE_H_

#include <string>
#include <vector>

class Nanostate
{
public:
  explicit Nanostate(const char *identity, const char *addr);
  ~Nanostate();
public:
  bool send_state_update(const std::string &name, const std::vector<char> &data);
  bool recv_state_update(std::string &sender, std::string &name, std::vector<char> &data);
  bool has_state_update();
  const std::string &get_identity() const { return _identity; }

  bool send_state_update(const std::string &name, const std::string &data);
  bool recv_state_update(std::string &sender, std::string &name, std::string &data);
protected:
  std::string _identity;
  int _sock;
};

#endif  // _NANOSTATE_H_
