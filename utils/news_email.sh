#!/bin/sh
# This script is used to send an email to all users 
# (unless NONEWS is defined)
# Set SEND to the sendmail binary path
SEND="/usr/sbin/sendmail -f\"noreply@ptlink.net\""

if [ $# -eq 0 ] ; then
  echo "Usage: $0 message"
  exit 1
fi
if [ ! -f $1 ] ; then
  echo "File $1 was not found !"
  exit 2
fi
tmpfile=/tmp/$$
echo "Generating nicks/email list"
./svsquery.sh sql/news_emails.sql > $tmpfile
# first line flag, to skip header
fl=1
while read line
do
  if [ $fl -eq 0 ] ; then
    nick=`echo $line | cut -d" " -f1`
    email=`echo $line | cut -d" " -f2`
    echo "Sending email to $nick <$email>"
    cat $1 | sed "s/%nick%/$nick/g" | sed "s/%email%/$email/g" | $SEND $email
  else
    fl=0
  fi
done < $tmpfile 
rm $tmpfile    
