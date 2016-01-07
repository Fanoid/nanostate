#!bin/bash
killall hub test1 test2
ADDR=tcp://127.0.0.1:15000
# bin/hub -b $ADDR &
bin/test1 -i test1 -c $ADDR &
bin/test2 -i test2 -c $ADDR &
