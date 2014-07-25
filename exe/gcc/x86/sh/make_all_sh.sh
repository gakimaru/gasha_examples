#!/bin/sh

echo "----------------------------------------"
echo [ `realpath ${0%/*}` ]

function copy()
{
	template=_template.sh
	sh=${1##*/}
	sh=${sh%.*}.sh
	if [ ! -f $sh ]; then
		echo cp $template $sh
		cp $template $sh
	fi
}

for prog in `ls ../example_*`
do
	copy $prog
done

