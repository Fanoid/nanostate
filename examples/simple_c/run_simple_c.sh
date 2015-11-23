#!bin/bash
killall hub test1 test2
ADDR=tcp://127.0.0.1:15000
bin/hub hub $ADDR &
bin/test1 test1 $ADDR &
bin/test2 test2 $ADDR &
