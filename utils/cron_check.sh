#!/bin/sh
#
# PTlink Services Crontab 
#
# This is a script suitable for use in a crontab.  It checks to make sure
# your ircsvs are running.  
# If your ircsvs isn't found, it'll try to start it back up.
#
# You'll need to edit this script for your ircsvs.
#
# To check for your ircsvs every 10 minutes, put the following line in your
# crontab:
#    0,10,20,30,40,50 * * * *   /home/mydir/ircsvs/ircsvschk
# And if you don't want to get email from crontab when it checks you ircsvs,
# put the following in your crontab:
#    0,10,20,30,40,50 * * * *   /home/mydir/ircsvs/ircsvschk >/dev/null 2>&1
#
ulimit -c unlimited
# change this to your ircsvs binary directory:
bdir="$HOME/ircsvs/bin"

#change this to your data directory
ddir="$HOME/ircsvs/var/run"

# change this to the name of your ircsvs's file in that directory:
ircsvsexe="ircsvs"

# I wouldn't touch this if I were you.
ircsvsname="$ddir/ircsvs.pid"

########## you probably don't need to change anything below here ##########

if test -r $ircsvsname; then
  # there is a pid file -- is it current?
  ircsvspid=`cat $ircsvsname`
  if `kill -CHLD $ircsvspid >/dev/null 2>&1`; then
    # it's still going
    # back out quietly
    exit 0
  fi
  echo ""
  echo "Stale $ircsvsname file (erasing it)"
  rm -f $ircsvsname
fi
echo ""
echo "Couldn't find the ircsvs running.  Reloading it..."
echo ""
cd $bdir
./$ircsvsexe

