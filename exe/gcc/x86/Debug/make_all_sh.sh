#!/bin/sh

function run_all_sub()
{
	sub_dir=$1
	if [ -f $sub_dir/make_all_sh.sh ]; then
		pushd $sub_dir > /dev/null
		./make_all_sh.sh
		popd > /dev/null
	fi
}

for sub_dir in `ls -F|grep /`
do
	run_all_sub $sub_dir
done
