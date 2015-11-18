from __future__ import print_function
import nanomsg
from nanomsg import wrapper as nn_wrapper
import msgpack
import sys

class Nanostate:
    def __init__(self, identity, addr):
        self._sock = nn_wrapper.nn_socket(nanomsg.AF_SP, nanomsg.BUS)
        nn_wrapper.nn_connect(self._sock, addr)
        self._identity = identity

    def send_state_update(self, name, data):
        buf = msgpack.packb(self._identity, name, data)
        nn_wrapper.nn_send(self._sock, buf, 0)

    def recv_state_update(self):
        result, buf = nn_wrapper.nn_recv(self._sock, 0)
        return msgpack.unpackb(buf)

nanostate = Nanostate(sys.argv[1], sys.argv[2])
while True:
    sender, name, data = nanostate.recv_state_update()
    print(sender, name, data)
