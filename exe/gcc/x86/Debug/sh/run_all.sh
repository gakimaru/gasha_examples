#!/bin/sh

echo "----------------------------------------"
echo [ `realpath ${0%/*}` ]

function run()
{
	script=$1
	./$script
}

for script in `ls example_*.sh`
do
	run $script
done
