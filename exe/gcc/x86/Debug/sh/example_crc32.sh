#!/bin/sh 

script_name=${0##./}
script_name=${script_name%.*}
prog_path=../$script_name
log_path=./log/$script_name.log

$prog_path > $log_path
