#!/bin/bash

echo "16" > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio16/direction

#while [ true ];
#do
	cat /sys/class/gpio/gpio16/value
#	sleep 1
#done
