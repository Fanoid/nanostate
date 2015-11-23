#!bin/bash
killall hub viewer3d
ADDR=tcp://192.168.40.13:12306
bin/hub hub $ADDR &
bin/viewer3d node1 $ADDR &
bin/viewer3d node2 $ADDR &
