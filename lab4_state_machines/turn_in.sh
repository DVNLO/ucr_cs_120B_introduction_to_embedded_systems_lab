#!/usr/bin/env bash
if [ "$#" -ne 2 ]; then
    echo "You must enter exactly 2 arguments"
    exit 1
fi
cp -i ./source/main.c ./turnin/dvyen001_lab$1_part$2.c \
&& \
cp -i ./test/tests.gdb ./turnin/dvyen001_lab$1_part$2_tests.gdb ;
