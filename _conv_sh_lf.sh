#!/bin/sh

echo nkf -Lu --oc=UTF-8 --overwrite $1
nkf -Lu --oc=UTF-8 --overwrite $1

echo chmod +x $1
chmod +x $1
