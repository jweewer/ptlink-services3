#!/bin/sh
# For this script to work services need to be installed on the default dir
ts=`date +"%Y%m%d_%H%M"`
dbhost=`grep '^DB_Host' ~/ircsvs/etc/ircsvs.conf | awk ' { print $2 }'`
dbuser=`grep '^DB_User' ~/ircsvs/etc/ircsvs.conf | awk ' { print $2 }'`
dbpass=`grep '^DB_Pass' ~/ircsvs/etc/ircsvs.conf | awk ' { print $2 }'` 
dbname=`grep '^DB_Name' ~/ircsvs/etc/ircsvs.conf | awk ' { print $2 }'`
echo "SET FOREIGN_KEY_CHECKS = 0;" > ircsvs3_data_${ts}.sql
mysqldump -C --disable-keys --add-drop-table -h $dbhost -u $dbuser -p$dbpass $dbname >> ircsvs3_data_${ts}.sql
echo Compressing ircsvs3_data_${ts}.sql
gzip ircsvs3_data_${ts}.sql
echo Done - ircsvs3_data_${ts}.sql.gz

