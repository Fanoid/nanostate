var nano = require("nanomsg");
var argparse = require("argparse")

var parser = argparse.ArgumentParser({
  description: "A nanostate hub written by Node.js."
});
parser.addArgument(
    ["--bind", "-B"],
    {
      dest: "addr",
      defaultValue: "tcp://127.0.0.1:15000",
      help: "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'"
    });

var args = parser.parseArgs()
var addr = args.addr;

// Start socket hub
var sock = nano.socket("bus");
sock.bind(addr);
sock.sndbuf(1024 * 1024 * 1024);
sock.rcvbuf(1024 * 1024 * 1024);
sock.rcvmaxsize(-1);

sock.on("data", function (buf) {
  sock.send(buf);
});

console.log("Socket bound to " + addr);

