#! /bin/sh

rm -fv tags cscope.*

find . -name "*.h" -o -name "*.cpp" -o -name "*.c" > cscope.files
cscope -b -q
ctags -L cscope.files
