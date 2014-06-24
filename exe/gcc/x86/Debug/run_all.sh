#!/bin/sh

function run_all_sub()
{
	sub_dir=$1
	if [ -f $sub_dir/run_all.sh ]; then
		pushd $sub_dir > /dev/null
		./run_all.sh
		popd > /dev/null
	fi
}

#for sub_dir in `ls -F|grep /`
for sub_dir in sh
do
	run_all_sub $sub_dir
done
