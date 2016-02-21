#!/bin/sh
# This script is used to send an email to all users expiring in
# the number of days defined on sql/expire_emails.sql
# (unless NONEWS is defined)
# Set SEND to the sendmail binary path
SEND="/usr/sbin/sendmail -f\"noreply@pt-link.net\""

tmpfile=tmp/$$
echo "Generating nicks/email list"
./svsquery.sh sql/expire_emails.sql > $tmpfile
# first line flag, to skip header
fl=1
while read line
do
  if [ $fl -eq 0 ] ; then
    nick=`echo $line | cut -d" " -f1`
    email=`echo $line | cut -d" " -f2`
    echo "Sending email to $nick <$email>"
    cat mails/Expiring | sed "s/%nick%/$nick/g" | sed "s/%email%/$email/g" | $SEND $email
  else
    fl=0
  fi
done < $tmpfile 
rm $tmpfile    
