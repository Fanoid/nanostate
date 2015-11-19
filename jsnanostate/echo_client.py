# -*- coding: utf-8 -*-
from ws4py.client.threadedclient import WebSocketClient
import msgpack

class EchoClient(WebSocketClient):
    def received_message(self, m):
        print(type(m))
        if (m.is_binary):
            buf = m.data
            state = msgpack.unpackb(buf, use_list = False)
            print(state)
        else:
            print(str(m))

if __name__ == '__main__':
    try:
        ws = EchoClient('ws://127.0.0.1:9000/ws', protocols=['http-only', 'chat'])
        ws.connect()
        ws.run_forever()
    except KeyboardInterrupt:
        ws.close()
