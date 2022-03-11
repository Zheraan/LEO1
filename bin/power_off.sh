#!/bin/bash

x=0

while [ true ];
do
	res=$(/home/pi/bin/read_switch.sh)
	echo $res
	if [ $res -eq 1 ]; then
		x=$(($x+1))
	else
		x=$((0))
	fi
	if [ $x -eq 3 ]; then
		shutdown now
	fi
	sleep 1
done
