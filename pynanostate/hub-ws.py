from __future__ import print_function
from nanomsg import *

import cherrypy # to host a websocket hub

from ws4py.server.cherrypyserver import WebSocketPlugin, WebSocketTool
from ws4py.websocket import WebSocket
from ws4py.messaging import TextMessage

import argparse

parser = argparse.ArgumentParser(description="A nanostate hub with router between websocket written by Python.")
parser.add_argument('--bind', '-B', dest = "addr", required = True, help = "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'")

parser.add_argument("--ws_addr", dest = "ws_addr", required = True, help = "The address to bind websocket server, such as 127.0.0.1")
parser.add_argument("--ws_port", dest = "ws_port", type=int, required = True, help = "The port to bind websocket server, such as 15001")

args = parser.parse_args()
print(args)
addr = args.addr
ws_addr = args.ws_addr
ws_port = args.ws_port

class WebSocketHandler(WebSocket):
    def received_message(self, msg):
#        print("WebSocket received: " + repr(msg))
        cherrypy.engine.publish('websocket-broadcast', msg)

        if (msg.is_binary):
            buf = msg.data
            sock.send(buf)
        else:
            sock.send(str(msg))

class Root:
    def __init__(self, addr, port):
        self.addr = addr
        self.port = port

    @cherrypy.expose
    def index(self):
        return "Use " + self.addr + ":" + str(self.port) + " for WebSocket connection."

    @cherrypy.expose
    def ws(self):
        cherrypy.log("Handler created: %s" % repr(cherrypy.request.ws_handler))


# Initialize websocket hub
cherrypy.config.update({
    'server.socket_host': ws_addr,
    'server.socket_port': ws_port})
WebSocketPlugin(cherrypy.engine).subscribe()
cherrypy.tools.websocket = WebSocketTool()

cherrypy.tree.mount(Root(ws_addr, ws_port), "/", config={
    '/ws': {
        'tools.websocket.on': True,
        'tools.websocket.handler_cls': WebSocketHandler
        }
    })

# Start WebSocket hub
print("Starting WebSocket hub")
cherrypy.engine.start()

# Initialize nanostate hub
sock = Socket(BUS)
sock.bind(args.addr)
print("Nanostate socket bound to " + addr)

# Start nanostate hub
print("Starting nanostate hub")
while True:
    buf = sock.recv()
#    print("nanostate hub received: " + repr(buf))
    sock.send(buf)
    cherrypy.engine.publish('websocket-broadcast', buf, binary = True)
#    print("WebSocket hub sent: " + repr(buf))

sock.close()
cherrypy.engine.stop()
