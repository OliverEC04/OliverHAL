#!/bin/bash

arm-rpizw-gcc -o swLedBasic swLedBasic.c
scp swLedBasic root@10.9.8.2:/home/root/HALal/
ssh root@10.9.8.2 "cd HALal/ && ./swLedBasic"