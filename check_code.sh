#!/bin/sh

find . -regex ".*\.\(cpp\|h\|inl\)" -exec _check_code.sh {} \; | sort > check_code.txt
find . -regex ".*\(Makefile\).*" -exec _check_code.sh {} \; | sort >> check_code.txt
find . -regex ".*\.\(sh\)" -exec _check_code.sh {} \; | sort >> check_code.txt
find . -regex ".*\.\(bat\)" -exec _check_code.sh {} \; | sort >> check_code.txt
find exe -regex ".*\.\(log\\)" -exec _check_code.sh {} \; | sort >> check_code.txt
find exe -regex ".*\.\(json\)" -exec _check_code.sh {} \; | sort >> check_code.txt
