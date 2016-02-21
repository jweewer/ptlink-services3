#!/bin/sh
# script by dp

if [ $# -ne 2 ]
then
	echo "Usage: `basename $0` LANGUAGE FILE"
	exit
fi

LANGUAGE="$1"
FILE="$2"

# get the "master" strings (we use en_us) and the target language strings into files
grep '^+[^+].*(en_us)$' ${FILE} | sed 's/^+\(.*\)(en_us)$/\1/' > /tmp/tr_master
grep "^+[^+].*(${LANGUAGE})$" ${FILE} | sed "s/^+\(.*\)(${LANGUAGE})$/\1/" > /tmp/tr_lang

# diff them
DIFF=`diff -Naur /tmp/tr_master /tmp/tr_lang | grep ^[-][^-]`

for i in $DIFF
do
	echo "! ${i:1} is untranslated for language '$LANGUAGE'"
done

# clean up
rm -f /tmp/tr_{master,lang,diff}
