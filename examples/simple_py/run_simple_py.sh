#!bin/bash
killall python
ADDR=tcp://127.0.0.1:15000
python hub.py --bind $ADDR &
python nanostate.py --identity test1 --connect $ADDR &
python nanostate.py --identity test2 --connect $ADDR &
