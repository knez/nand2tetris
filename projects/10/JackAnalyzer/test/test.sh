#!/bin/bash

BASEDIR="../.."
TOOLS="../../../../tools"
UNITS="ArrayTest Square ExpressionLessSquare"

for unit in $UNITS
do
	../JackAnalyzer $BASEDIR/$unit >/dev/null 2>&1
	for file in $BASEDIR/$unit/*.xml
	do
		$TOOLS/TextComparer.sh $file $unit/$(basename $file) >/dev/null 2>&1
		if [ $? -eq 0 ]
		then
			echo -n "."
		else
			echo -n "F"
			FAILED="$file"
		fi
	done
done

echo ""
if [ ! -z "$FAILED" ]
then
	echo "Failed test: $FAILED"
else
	echo "Success!"
fi

