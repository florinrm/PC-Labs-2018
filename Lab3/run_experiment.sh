#!/bin/bash

SPEED=1
DELAY=1
LOSS=0
CORRUPT=100
FILE=$1

killall link 2> /dev/null
killall recv 2> /dev/null
killall send 2> /dev/null

./link_emulator/link speed=$SPEED delay=$DELAY loss=$LOSS corrupt=$CORRUPT &> /dev/null &
sleep 1
./recv &
sleep 1

./send $FILE
