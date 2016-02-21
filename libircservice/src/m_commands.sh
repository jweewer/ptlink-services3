#!/bin/sh
echo "Generating ../include/m_commands.h"
echo>../include/m_commands.h
cat *.c | egrep "void.m_" | sed "s/\[\])/\[\]);/g" | grep -v ";;" > ../include/m_commands.h

