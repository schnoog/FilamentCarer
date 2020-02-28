#!/bin/bash

MYIFS=$IFS 
IFS="
"

FUNC_HEADERS=$(grep void * | grep -v headers | grep -v README | cut -d ':' -f 2 | sed -e 's/{/;/g')

for FH in $FUNC_HEADERS
do

if [ $(grep -F "$FH" headers.cpp | wc -l) -eq 0 ]
then

echo "new $FH"
echo "$FH" >> headers.cpp

else

echo "old $FH"

fi
done

IFS=$MYIFS
