#!/bin/bash

year=$1
filname=$2
testMode=$3

if [ -z "$filname" ]; then
    echo "Usage: $0 <year> <day>"
    exit 1
fi

if [ -z "$year" ]; then
    echo "Usage: $0 <year> <day>"
    exit 1
fi

if [ "$filname" == "all" ]; then
    for i in {1..25}
    do
        if [ $i -lt 10 ]; then
            i="0$i"
        fi
        # if is a valid file
        if [ -f "./$year/$i.cpp" ]; then
            echo ""
            echo -e "\033[0;32mRunning day $i\033[0m"
            ./compile_run.sh $year $i $testMode
        fi
    done
    exit 0
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

g++ ./$file ./Util.cpp -o run.o && time ./run.o $inputFile $testMode