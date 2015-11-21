from __future__ import print_function
from nanomsg import *
import argparse

parser = argparse.ArgumentParser(description="A nanostate hub written by Python.")
parser.add_argument('--bind', '-B', dest = "addr", required = True, help = "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:12306'")

args = parser.parse_args()
addr = args.addr

sock = Socket(BUS)
sock.bind(args.addr)
print("Socket bound to " + addr)

while True:
    buf = sock.recv()
    sock.send(buf)

sock.close()
