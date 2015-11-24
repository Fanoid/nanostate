#!bin/bash
killall python
ADDR=tcp://127.0.0.1:15000
WS_HOST=127.0.0.1
WS_PORT=15001
WS_PATH=ws://$WS_HOST:$WS_PORT/ws

python hub-ws.py --bind $ADDR --ws_host $WS_HOST --ws_port $WS_PORT &
sleep 1
python nanostate.py --identity py --connect $ADDR &
python echo_client.py --identity ws --connect $WS_PATH &
