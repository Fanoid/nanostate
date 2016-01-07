#include "cnanostate/nanostate.h"

#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <msgpack.hpp>
#include <sstream>
#include <iostream>

using std::string;
using std::stringstream;

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
  if ((identity == NULL) || (addr == NULL))
  {
    fprintf(stderr, "Usage: %s -i IDENTITY -c ADDR\n", argv[0]);
    fprintf(stderr, "IDENTITY is a string to identify this client\n");
    fprintf(stderr, "ADDR is the address of the server, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'\n");
    return 1;
  }

  Nanostate nanostate(identity, addr);
  string sender, name, data;
  while (true)
  {
    if (nanostate.recv_state_update(sender, name, data) && (sender != identity))
    {
      msgpack::unpacked result;
      msgpack::unpack(result,data.data(),data.size());

      msgpack::object deserialized = result.get();
      msgpack::type::tuple<vector<float> >dst;
      deserialized.convert(&dst);
      vector<float> vec1 = dst.get<0>();
      for(int i=0;i<vec1.size();i++){
        fprintf(stderr,"%f,",vec1[i]);
      }
      fprintf(stderr, "%s receive state update from %s: %s: %d\n", identity, sender.c_str(), name.c_str(), vec1.size());

      // fprintf(stderr, "%s receive state update from %s: %s: %s\n", identity, sender.c_str(), name.c_str(), data.c_str());

      // vector<float> vec(1.3,2.222,4.55,2.3333);
     float x[10] = {1,4,4,5,7,7,8,9,9,10};
     vector<float> vec(x,x+sizeof x/ sizeof x[0]);

     // string strr = "";
     // for(int i=0;i<vec.size();i++){
     //  strr+=to_string(vec[i]);
     //  strr+=",";
     // }
     //  nanostate.send_state_update("Hello",strr);

     // // nanostate.send_state_update("hello1",strr);

      msgpack::type::tuple<vector<float> >src(vec);
      stringstream buffer;
      msgpack::pack(buffer, src);
      nanostate.send_state_update("hello1", buffer.str());


      // vector<float> vec({2.1,3.3,4.0});
      // msgpack::type::vector<float> src(vec);
      // stringstream buffer;
      // msgpack::pack(buffer,src);

      // nanostate.send_state_update("hello11", buffer.str());
    }
  }

  return 0;
}
