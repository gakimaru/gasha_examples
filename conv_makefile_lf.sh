#!/bin/sh

find . -name "Makefile*" -exec _conv_makefile_lf.sh {} \;
