#!/bin/bash
sudo sysctl -w net.ipv4.ip_forward=1
#sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables-restore < /home/pi/bin/iptables.rules.v4

/home/pi/bin/led_blink.sh &
/home/pi/bin/power_off.sh &
