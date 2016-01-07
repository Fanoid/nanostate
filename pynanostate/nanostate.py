from __future__ import print_function
from nanomsg import *
import nanomsg
import msgpack
import sys
import time
import argparse

class Nanostate:
    def __init__(self, identity, addr):
        self._sock = Socket(BUS)
        self._sock.connect(addr)
        self._sock.recv_timeout = 50
        self._sock.send_buffer_size = 1024 * 1024 * 1024
        self._sock.recv_buffer_size = 1024 * 1024 * 1024
        self._sock.set_int_option(SOL_SOCKET, RCVMAXSIZE, -1)
        self._identity = identity

    def send_state_update(self, name, data):
        if (type(data) is str): data = data.encode('utf-8')
        state = (self._identity, name, data)
        buf = msgpack.packb(state, use_bin_type = True)
        try:
            self._sock.send(buf)
#            print(self._identity + " sent: " + str(state))
            return True
        except NanoMsgError:
            return False

    def recv_state_update(self):
        state = None
        try:
            buf = self._sock.recv()
            state = msgpack.unpackb(buf, use_list = False, encoding = 'utf-8')
            if state[0] != self._identity:
#                print(self._identity + " received: " + str(state))
                return (True, state)
            else:
                return (False, state)
        except NanoMsgError:
            return (False, state)

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="A nanostate client written by Python.")
    parser.add_argument('--identity', '-I', dest = "identity", required = True, help = "A string to identiy this client")
    parser.add_argument('--connect', '-C', dest = "addr", required = True, help = "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'")

    args = parser.parse_args()
    print(args)
    identity = args.identity
    addr = args.addr

    nanostate = Nanostate(identity, addr)
    while True:
        time.sleep(1)
        nanostate.send_state_update("Hello", ("我是" + identity).encode("utf-8"))
        result, state = nanostate.recv_state_update()
        if result:
            print(identity + " received: ", state[1], state[2])
