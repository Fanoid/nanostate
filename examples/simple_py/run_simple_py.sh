#!bin/bash
killall python
ADDR=tcp://127.0.0.1:15000
python hub.py --bind $ADDR &
python nanostate.py test1 $ADDR &
python nanostate.py test2 $ADDR &
