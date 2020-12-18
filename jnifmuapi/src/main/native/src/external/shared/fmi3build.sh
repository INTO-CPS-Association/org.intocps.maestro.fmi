#!/bin/bash


types=$(grep "TYPE" fmi3/headers/fmi3FunctionTypes.h | sed 's/const char/constchar/g'| sed 's/  */ /g' |cut -d ' ' -f3 | cut -d '(' -f1)


for file in $types ; do

name=${file::${#file}-4}

   echo "$file        * $name;" 
done



for file in $types ; do

name=${file::${#file}-4}

   echo "fmu->$name = ($file *)getAdr(&s, h, \"$name\");" 
done
