from __future__ import print_function
from nanomsg import *
import argparse

parser = argparse.ArgumentParser(description="A nanostate hub written by Python.")
parser.add_argument('--bind', '-B', dest = "addr", default = "tcp://*:15000", help = "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'. Bind to 'tcp://*:15000' by default.")

args = parser.parse_args()
addr = args.addr

sock = Socket(BUS)
sock.bind(args.addr)
sock.send_buffer_size = 1024 * 1024 * 1024
sock.recv_buffer_size = 1024 * 1024 * 1024
sock.set_int_option(SOL_SOCKET, RCVMAXSIZE, -1)
print("Socket bound to " + addr)

while True:
    buf = sock.recv()
    sock.send(buf)

sock.close()
