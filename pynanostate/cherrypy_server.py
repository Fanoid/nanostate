# -*- coding: utf-8 -*-
import argparse
import random
import os

import cherrypy

class Root(object):
    def __init__(self, ws_host, ws_port):
        self.ws_host = ws_host
        self.ws_port = ws_port

    @cherrypy.expose
    def index(self):
        return """<html>
    <head>
      <script type='application/javascript' src='/js/jquery.min.js'></script>
      <script type='application/javascript' src='/js/msgpack.min.js'></script>
      <script type='application/javascript' src='/js/nanostate.js'></script>
      <script type='application/javascript'>
        $(document).ready(function() {
          identity = "%(username)s"

          websocket = 'ws://%(ws_host)s:%(ws_port)s/ws';
          if (window.WebSocket) {
            ws = new WebSocket(websocket);
          }
          else if (window.MozWebSocket) {
            ws = MozWebSocket(websocket);
          }
          else {
            console.log('WebSocket Not Supported');
            return;
          }
          ws.binaryType = 'arraybuffer'

          window.onbeforeunload = function(e) {
            $('#chat').val($('#chat').val() + 'Bye bye...\\n');
            ws.close(1000, '%(username)s left the room');

            if(!e) e = window.event;
            e.stopPropagation();
            e.preventDefault();
          };
          ws.onmessage = function (e) {
             if (typeof(e.data) === "string")
             {
                $('#chat').val($('#chat').val() + e.data + '\\n');
             }
             else
             {
                state = recv_state_update(e.data);
                $('#chat').val($('#chat').val() + 'state: ' + 
                JSON.stringify(state) + '\\n');
             }
          };
          ws.onopen = function() {
          };
          ws.onclose = function(e) {
          };

          $('#send').click(function() {
             send_state_update(ws, '%(username)s', 'Hello', $('#message').val());
             $('#message').val("");
             return false;
          });
        });
      </script>
    </head>
    <body>
    <form action='#' id='chatform' method='get'>
      <textarea id='chat' cols='80' rows='40'></textarea>
      <br />
      <label for='message'>%(username)s: </label><input type='text' id='message' />
      <input id='send' type='submit' value='Send' />
      </form>
    </body>
    </html>
    """ % {'username': "User%d" % random.randint(0, 100), 'ws_host': self.ws_host, 'ws_port': self.ws_port}

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A web server writting by Python CherryPy, which will connect to a WebSocket server')
    parser.add_argument("--http_host", dest = "http_host", required = True, help = "The host to host a web server")
    parser.add_argument("--http_port", dest = "http_port", required = True, type=int, help = "The port to host a web server")
    parser.add_argument("--ws_host", dest = "ws_host", required = True, help = "The host of the websocket server, such as 127.0.0.1")
    parser.add_argument("--ws_port", dest = "ws_port", type=int, required = True, help = "The port of the websocket server, such as 15001")
    args = parser.parse_args()

    cherrypy.config.update({'server.socket_host': args.http_host,
                            'server.socket_port': args.http_port,
                            'tools.staticdir.root': os.path.abspath(os.path.dirname(__file__))})

    cherrypy.quickstart(Root(args.ws_host, args.ws_port), '', config={
        '/js': {
              'tools.staticdir.on': True,
              'tools.staticdir.dir': 'js'
            }
        }
    )
