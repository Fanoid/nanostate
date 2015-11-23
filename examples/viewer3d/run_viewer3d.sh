#!bin/bash
killall hub viewer3d
ADDR=tcp://127.0.0.1:15000
bin/hub -b $ADDR &
bin/viewer3d -i node1 -c $ADDR &
bin/viewer3d -i node2 -c $ADDR &
