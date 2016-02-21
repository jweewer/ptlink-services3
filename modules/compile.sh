#!/bin/sh
if [ ! $# -eq 1 ]
then
  echo "Usage: $0 module.c"
  exit
fi
output=`echo $1 | sed "s/\.c/\.so/g"`
echo "Compiling module $1"
CC=gcc
INCLUDES="-I../include -I../libdconf -I../libircservice/include"
CFLAGS="-g -Wshadow -Wunused -Wall -Wmissing-declarations -ansi ${INCLUDES}"
PICFLAGS="-fPIC -DPIC -shared"
CPPFLAGS=" -I/usr/include/mysql"
echo ${CC} ${PICFLAGS} ${CPPFLAGS} ${CFLAGS} $1 -o $output
