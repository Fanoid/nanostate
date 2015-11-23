#!bin/bash
killall python
ADDR=tcp://127.0.0.1:15000
WS_ADDR=127.0.0.1
WS_PORT=15001
WS_PATH=ws://$WS_ADDR:$WS_PORT/ws

python hub-ws.py --bind $ADDR --ws_addr $WS_ADDR --ws_port $WS_PORT &
python nanostate.py --identity py --connect $ADDR &
sleep 1
python echo_client.py --identity ws --connect $WS_PATH &
