#! /bin/bash
RED='\003[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

try(){
    expected="$1"
    input="$2"

    ./tmcc "$input" > tmp.s
    gcc -static -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo -e $GREEN"$input => $actual"$NC
    else
        echo -e $RED"$expected expected, but got $actual"$NC
        exit 1
    fi
}

try_fail() {
    input="$1"

    ./tmcc "$input" > /dev/null 2>/dev/null
    result="$?"

    if [ "$result" == "0" ]; then
        echo -e $RED"not failed: $input"$NC
        exit 1
    else
        echo -e $GREEN"$input failed as expected."$NC
    fi
}

try 41 ' 12 + 34 - 5;'

try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"

try_fail '();'
try_fail '1+(2+ );'
try_fail '1 1;'

echo OK
