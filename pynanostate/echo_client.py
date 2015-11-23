# -*- coding: utf-8 -*-
import nanostate

from ws4py.client.threadedclient import WebSocketClient

import msgpack
import argparse

class EchoClient(WebSocketClient):
    def __init__(self, identity, url, **kargs):
        print(args)
        self._identity = identity
        WebSocketClient.__init__(self, url, kargs)

    def received_message(self, m):
        if (m.is_binary):
            buf = m.data
            state = msgpack.unpackb(buf, use_list = False)
            if state[0].decode() != self._identity:
                print(self._identity + " received: " + str(state))

                state = (self._identity, "你好", "世界")
                buf = msgpack.packb((self._identity, "你好", "世界"))
                self.send(buf, binary = "True")
                print(self._identity + " sent: " + str(state))
        else:
            print(str(m))
            self.send(b"World!", binary = True)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="A WebSocket client written by Python.")
    parser.add_argument('--identity', '-I', dest = "identity", required = True, help = "A string to identiy this client")
    parser.add_argument("--connect", dest = "path", required = True, help = "The address of WebSocket server, such as ws://127.0.0.1:15001/ws")

    args = parser.parse_args()
    print(args)
    identity = args.identity
    path = args.path

    try:
        ws = EchoClient(identity, path, protocols=['http-only', 'chat'])
        ws.connect()
        ws.run_forever()
    except KeyboardInterrupt:
        ws.close()
