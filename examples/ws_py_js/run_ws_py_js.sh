#!bin/bash
killall python

ADDR=tcp://127.0.0.1:15000

WS_HOST=127.0.0.1
WS_PORT=15001
WS_PATH=ws://$WS_HOST:$WS_PORT/ws

HTTP_HOST=127.0.0.1
HTTP_PORT=9000

python hub-ws.py --bind $ADDR --ws_host $WS_HOST --ws_port $WS_PORT &
python nanostate.py --identity py --connect $ADDR &
sleep 1
python cherrypy_server.py --http_host=$HTTP_HOST --http_port=$HTTP_PORT --ws_host=$WS_HOST --ws_port=$WS_PORT &
chromium $HTTP_HOST:$HTTP_PORT
