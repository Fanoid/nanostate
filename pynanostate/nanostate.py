from __future__ import print_function
import nanomsg
import msgpack
import sys

class Nanostate:
    def __init__(self, identity, addr):
        self._sock = nanomsg.Socket(nanomsg.BUS)
        self._sock.connect(addr)
        self._identity = identity

    def send_state_update(self, name, data):
        buf = msgpack.packb(self._identity, name, data)
        self._sock.send(buf)

    def recv_state_update(self):
        buf = self._sock.recv()
        return msgpack.unpackb(buf)

    def has_state_update(self): # Not work
        return (self._sock.recv_fd == 
                self._sock.get_int_option(nanomsg.SOL_SOCKET, nanomsg.RCVFD))

nanostate = Nanostate(sys.argv[1], sys.argv[2])
while True:
    if (nanostate.has_state_update()):
        print("received!")
        sender, name, data = nanostate.recv_state_update()
        print(sender, name, data)
    else:
        print("No!")
