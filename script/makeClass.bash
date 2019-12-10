#!/bin/bash

if [ "$#" -ne 4 ]; then
  echo "usage: $0 <template file> <source dir> <include dir> <ClassName>"
  exit 1
fi

templateFile=$1
sourceDir=$2
includeDir=$3
className=$4
lowerClassName=$(echo "$4" | awk '{print tolower($0)}')
sourceFile="$sourceDir/$lowerClassName.cpp"
includeFile="$includeDir/$lowerClassName.h"

relativePath=$(realpath --relative-to="$sourceDir" "$includeFile")

if [ -f "$sourceFile" ]; then
  echo "$sourceFile exsists!"
  exit 1
fi

if [ -f "$includeFile" ]; then
  echo "$includeFile exsists!"
  exit 1
fi


echo "Creating $className in $sourceFile and $includeFile"
cat "$templateFile" > "$sourceFile"
echo -e "\n#include \"$relativePath\"\n" >> "$sourceFile"

cat "$templateFile" > "$includeFile"
echo -e "\nclass $className {};\n" >> "$includeFile"