from __future__ import print_function
from nanomsg import *
import nanomsg
import msgpack
import sys
import time

class Nanostate:
    def __init__(self, identity, addr):
        self._sock = Socket(BUS)
        self._sock.connect(addr)
        self._sock.recv_timeout = 50
        self._identity = identity

    def send_state_update(self, name, data):
        buf = msgpack.packb((self._identity, name, data))
        try:
            self._sock.send(buf)
            return True
        except NanoMsgError:
            return False

    def recv_state_update(self):
        state = None
        try:
            buf = self._sock.recv()
            state = msgpack.unpackb(buf, use_list = False)
            return (True, state)
        except NanoMsgError:
            return (False, state)

if __name__ == "__main__":
    nanostate = Nanostate(sys.argv[1], sys.argv[2])
    while True:
        time.sleep(1)
        nanostate.send_state_update("Hello", "World")
        result, state = nanostate.recv_state_update()
        if result:
            print(state)
