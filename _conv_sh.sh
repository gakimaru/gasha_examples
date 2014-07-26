#!/bin/sh

echo `nkf -Lu --overwrite $1` ... nkf -Lu --overwrite $1
echo `chmod +x $1` ... chmod +x $1
