var nano = require('nanomsg');
var argparse = require('argparse')
var WebSocketServer = require('ws').Server

var parser = argparse.ArgumentParser({
  description: "A nanostate hub with router between websocket written by Node.js."
});
parser.addArgument(
    ['--bind', '-B'],
    {
      dest: "addr",
      required: true,
      help: "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'"
    });
parser.addArgument(
    ["--ws_host"],
    {
      dest: "ws_host",
      required: true,
      help: "The address to bind websocket server, such as 127.0.0.1"
    });
parser.addArgument(
    ["--ws_port"],
    {
      dest: "ws_port",
      type: "int",
      required: true,
      help: "The port to bind websocket server, such as 15001"
    });

var args = parser.parseArgs();
var addr = args.addr;
var ws_host = args.ws_host;
var ws_port = args.ws_port;

var sock = nano.socket('bus');
sock.bind(addr);
sock.sndbuf(1024 * 1024 * 1024);
sock.rcvbuf(1024 * 1024 * 1024);
sock.rcvmaxsize(-1);
console.log("Nanostate socket bound to " + addr);

var wss = new WebSocketServer({
  host: ws_host,
  port: ws_port,
  path: "/ws"
});

wss.broadcast = function broadcast(data) {
  wss.clients.forEach(function each(client) {
    client.send(data);
  });
};

wss.on('connection', function connection(ws) {
  console.log("1 client connected.");
  ws.on('message', function(data, flags) {
    console.log('ws received: %s', data);
    wss.broadcast(data);
    sock.send(data);
  });
});

sock.on('data', function (buf) {
  console.log('ws received: %s', buf);
  sock.send(buf);
  wss.broadcast(buf);
});

