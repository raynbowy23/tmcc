#! /bin/bash
RED='\003[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

try(){
    expected="$1"
    input="$2"
    msg="$3"

    ./tmcc "$input" > tmp.s
    if [ $? != 0 ]; then
        echo "$3: cann't compile"
        exit 1
    fi

    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" == "$expected" ]; then
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

try 42 'return 42;'
try 41 'return 12 + 34 - 5;'

try 47 "return 5+6*7;"
# try 40 "return 10*(3+1);"
# try 3 "return 9%6;"
# try 15 "return 5*(9-6);"
# try 4 "return (3+5)/2;"
# try 1 "return 2==2;"
# try 1 "return 1&&1;"
# try 0 "return 0||0;"

try_fail '();'
try_fail '1+(2+ );'
try_fail '1 1;'

echo OK
