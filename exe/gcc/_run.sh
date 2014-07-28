#!/bin/sh

this_script_dir=`realpath ${0%/*}`
this_script_dir__=$this_script_dir/_

script_name=${1##*/}
script_name=${script_name%.*}
script_dir=`realpath ${1%/*}`
script_dir__=$script_dir/_

prog_path=`realpath $script_dir/../$script_name`
log_path=`realpath -m $script_dir/log/$script_name.log`

print_prog_path=`echo $prog_path|cut -b ${#this_script_dir__}-`
print_log_path=`echo $log_path|cut -b ${#script_dir__}-`

echo \$ $print_prog_path \> $print_log_path
$prog_path > $log_path
