#!/bin/bash

FAILED=""

for testfile in ./*.sh
do
	if [ "$testfile" != "$0" ]
	then
		if $testfile
		then
			echo -n "."
		else
			echo -n "F"
			FAILED="$testfile"
		fi
	fi
done

echo ""
if [ ! -z "$FAILED" ]
then
	echo "Failed test: $FAILED"
fi
