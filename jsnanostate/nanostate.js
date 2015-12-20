function send_state_update(ws, identity, name, data)
{
  buf = msgpack.encode([identity, name, data]);
  ws.send(buf);
}

function recv_state_update(buf)
{
  state = msgpack.decode(new Uint8Array(buf))
  info = {
    "sender" : state[0],
    "name" : state[1],
    "data" : state[2]
  };
  return info;
}

function start_websocket(identity, ws_host, ws_port, ws_path, onmessage_callback, onopen_callback, onclose_callback)
{
    ws = new WebSocket("ws://" + ws_host + ":" + ws_port + ws_path);
    ws.identity = identity;
    ws.binaryType = "arraybuffer";
    ws.onmessage = onmessage_callback;
    ws.onopen = onopen_callback;
    ws.onclose = onclose_callback;
    return ws;
}
