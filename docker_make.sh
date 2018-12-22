#!/bin/bash

set -eu

if [ ! -e ./Makefile ]; then
    echo "Error:Makefile not exists."
    exit 1
fi

eval docker build -t tmcc . >/dev/null
eval docker run -it --rm --name tmcc tmcc
eval docker rmi tmcc >/dev/null
