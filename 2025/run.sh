#!/bin/bash
day=$1
testMode=$2

# check if day == all
if [ "$day" == "all" ]; then
    for dir in $(ls -d */); do
    # check if dir is numeric
        if [ -d "$dir" ]; then
            if [ -f "$dir/main.ts" ]; then
                echo "Running day $dir"
                (cd $dir && npx tsx main.ts ${testMode})
                echo "--------------------------------"
                echo ""
            fi
        fi
    done
    exit 0
fi

if [ -z "$day" ]; then
    echo "Usage: $0 <day>"
    exit 1
fi

cd ${day} && npx tsx main.ts ${testMode}