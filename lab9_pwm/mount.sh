#!/usr/bin/env bash
if [ "$#" -ne 2 ]; then
    echo "You must enter exactly 2 arguments"
    exit 1
fi
cp -i ./turnin/dvyen001_lab$1_part$2.c ./source/main.c \
&& \
cp -i ./turnin/dvyen001_lab$1_part$2_tests.gdb ./test/tests.gdb ;
