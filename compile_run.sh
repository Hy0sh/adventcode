#!/bin/bash

year=$1
filname=$2
testMode=$3

if [ -z "$filname" ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

file="${year}/${filname}.cpp"
inputFile="./${year}/inputs/${filname}"

if [ "$testMode" == "test" ]; then
    inputFile="${inputFile}_test"
fi

inputFile="${inputFile}.txt"

if [ ! -f "$inputFile" ]; then
    echo "Input file not found: $inputFile"
    exit 1
fi

if [ ! -f "$file" ]; then
    echo "File not found: $file"
    exit 1
fi

g++ ./$file ./Util.cpp ./Grid.cpp -o run.o && ./run.o $inputFile