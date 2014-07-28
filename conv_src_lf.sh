#!/bin/sh

find . -name "*.cpp" -exec _conv_src_lf.sh {} \;
find . -name "*.h" -exec _conv_src_lf.sh {} \;
find . -name "*.inl" -exec _conv_src_lf.sh {} \;
