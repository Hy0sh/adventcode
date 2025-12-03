#!/bin/bash
day=$1
testMode=$2

if [ -z "$day" ]; then
    echo "Usage: $0 <day>"
    exit 1
fi

cd ${day} && npx tsx main.ts ${testMode}