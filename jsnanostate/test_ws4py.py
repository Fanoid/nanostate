from ws4py.websocket import WebSocket
from ws4py.messaging import TextMessage

class EchoWebSocket(WebSocket):
    def received_message(self, message):
        self.send(message.data, message.is_binary)


def data_source():
    yield TextMessage(u'hello world')

from mock import MagicMock
source = MagicMock(side_effect=data_source)
ws = EchoWebSocket(sock=source)
ws.send(u'hello there')
