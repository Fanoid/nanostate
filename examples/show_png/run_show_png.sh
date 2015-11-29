#!bin/bash
killall python

ADDR=tcp://127.0.0.1:15000

WS_HOST=127.0.0.1
WS_PORT=15001
WS_PATH=ws://$WS_HOST:$WS_PORT/ws

HTTP_HOST=127.0.0.1
HTTP_PORT=9000

# Step 1: start a nanostate server
python hub-ws.py --bind $ADDR --ws_host $WS_HOST --ws_port $WS_PORT &
# Step 2: Start a web server, and open the page: index.html
# Step 3: send a png file using send_png.py: python send_png.py --identity png --connect tcp://127.0.0.1:15000 --png-filename ${PNG_FILENAME}
