#!/bin/bash

echo 12 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio12/direction
cat /sys/class/gpio/gpio12/value