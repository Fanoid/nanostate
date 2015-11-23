# -*- coding: utf-8 -*-
from ws4py.client.threadedclient import WebSocketClient
import msgpack

import argparse

class EchoClient(WebSocketClient):
    def received_message(self, m):
        print(type(m))
        if (m.is_binary):
            buf = m.data
            state = msgpack.unpackb(buf, use_list = False)
            print(state)
        else:
            print(str(m))
            self.send("World!")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="A WebSocket echo client.")
    parser.add_argument("--connect", dest = "addr", required = True, help = "The address of WebSocket server, such as ws://127.0.0.1:15001/ws")

    args = parser.parse_args()
    print(args)
    addr = args.addr

    try:
        ws = EchoClient(addr, protocols=['http-only', 'chat'])
        ws.connect()
        ws.run_forever()
    except KeyboardInterrupt:
        ws.close()
