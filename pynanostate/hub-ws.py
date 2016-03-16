from __future__ import print_function
from nanomsg import *

import cherrypy # to host a websocket hub

from ws4py.server.cherrypyserver import WebSocketPlugin, WebSocketTool
from ws4py.websocket import WebSocket
from ws4py.messaging import TextMessage

import argparse

parser = argparse.ArgumentParser(description="A nanostate hub with router between websocket written by Python.")
parser.add_argument('--bind', '-B', dest = "addr", default = "tcp://*:15000", help = "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'. Bind to 'tcp://*:15000' by default.")

parser.add_argument("--ws_host", dest = "ws_host", default = "0.0.0.0", help = "The address to bind websocket server, such as '127.0.0.1'. Bind to '0.0.0.0' (all interfaces) by default.")
parser.add_argument("--ws_port", dest = "ws_port", type=int, default = 15001, help = "The port to bind websocket server, such as '15001'. Bind to '15001' by default.")

args = parser.parse_args()
print(args)
addr = args.addr
ws_host = args.ws_host
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
    'server.socket_host': ws_host,
    'server.socket_port': ws_port})
WebSocketPlugin(cherrypy.engine).subscribe()
cherrypy.tools.websocket = WebSocketTool()

cherrypy.tree.mount(Root(ws_host, ws_port), "/", config={
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
sock.send_buffer_size = 1024 * 1024 * 1024
sock.recv_buffer_size = 1024 * 1024 * 1024
sock.set_int_option(SOL_SOCKET, RCVMAXSIZE, -1)
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
