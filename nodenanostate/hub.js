var nano = require('nanomsg');
var argparse = require('argparse')

var parser = argparse.ArgumentParser({
  description: "A nanostate hub written by Python."
});
parser.addArgument(
    ['--bind', '-B'],
    {
      dest: "addr",
      required: true,
      help: "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'"
    });
var args = parser.parseArgs()

var sock = nano.socket('bus');
var addr = args.addr;

sock.bind(addr);
sock.sndbuf(1024 * 1024 * 1024);
sock.rcvbuf(1024 * 1024 * 1024);
sock.rcvmaxsize(-1);

console.log("Socket bound to " + addr);

sock.on('data', function (buf) {
  sock.send(buf);
});
