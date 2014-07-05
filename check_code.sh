#!/bin/sh

find -regex ".*\.\(cpp\|h\|inl\|sh\)" -exec _check_code.sh {} \; | sort > check_code.txt
