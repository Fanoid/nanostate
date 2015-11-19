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
