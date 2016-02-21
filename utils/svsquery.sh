#!/bin/sh
# For this script to work services need to be installed on the default dir
# -Lamego
if [ $# -eq 0 ] ; then
  echo "Usage: $0 file.sql";
  exit
fi
dbhost=`grep 'DB_Host' ~/ircsvs/etc/ircsvs.conf|awk '{print $2}'|tr -d \"`
dbuser=`grep 'DB_User' ~/ircsvs/etc/ircsvs.conf|awk '{print $2}'|tr -d \"`
dbpass=`grep 'DB_Pass' ~/ircsvs/etc/ircsvs.conf|awk '{print $2}'|tr -d \"`
dbname=`grep 'DB_Name' ~/ircsvs/etc/ircsvs.conf|awk '{print $2}'|tr -d \"`
mysql -h $dbhost -u $dbuser -p$dbpass $dbname < $1 
