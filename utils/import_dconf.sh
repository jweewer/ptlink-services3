#!/bin/sh
# This script will read a .dconf file and map the settings to the new
# dbconf option.
# The output will be a list of "ircsvs dconf set" commands that can be used
# to update the configuration
if [ ! $# -eq 1 ]; then
  echo "Usage: $0 file.dconf"
  exit 1
fi

SVSBASE="./ircsvs conf"
target=$1
find_string()
{
  line=`grep -v "^ *#.*" $target| grep "$1"`
  if [ -n "$line" ]; then
  	line=`echo "$line" | sed "s/^.*$1//g"`
    	line=`echo "$line" | sed "s/^[ \t]*//g"`
  	if [ `echo $line | grep -c "\"$"` -eq 0 ]; then
  		line=`echo $line\"`;
  	fi
  	if [ `echo $line | grep -c "^\""` -eq 0 ]; then
  		line=`echo \"$line`;
  	fi  	
  	echo "$SVSBASE set $2 $line"
  fi
}

find_int()
{
  line=`grep -v "^ *#.*" $target| grep "$1"`
  if [ -n "$line" ]; then
  	line=`echo "$line" | sed "s/^.*$1//g"`
    	line=`echo "$line" | sed "s/^[ \t]*//g"`  	
	line=`echo "$line" | sed "s/^.*[^0-9][hmsMY]//g"`
  	echo "$SVSBASE set $2 $line"
  fi
}

find_switch()
{
  line=`grep -v "^ *#.*" $target| grep "$1"`
  if [ -n "$line" ]; then
  	line=`echo "$line" | sed "s/^.*$1//g"`
    	line=`echo "$line" | sed "s/^[ \t]*//g"`
	line=`echo "$line" | sed "s/[Yy][Ee][Ss]/On/g"`
	line=`echo "$line" | sed "s/[Nn][No]/Off/g"`
  	echo "$SVSBASE set $2 $line"
  fi
}

find_string "ServerName" "irc.ServerName"
find_string "ServerDesc" "irc.ServerDesc"
find_string "ServerPass" "irc.ServerPass"
find_string "RemoteServer" "irc.RemoteServer"
find_int "RemotePort" "irc.RemotePort"
find_int "SecurityCodeLenght" "nickserv.SecurityCodeLenght"
find_int "ExpireInterval" "expire.Interval" 
find_string "NS_Nick" "nickserv.Nick"
find_string "NS_Username" "nickserv.Username"
find_string "NS_Host" "nickserv.Hostname"
find_string "NS_Info" "nickserv.Realname"
find_string "NS_LogChan" "nickserv.LogChan"
find_int "MaxNicksPerMail" "nickserv.MaxNicksPerEmail"
find_int "FailedIdentifyAttempts" "nickserv.FailedLoginMax"
find_string "NickProtectionPrefix" "nickserv.NickProtectionPrefix"
find_int "MaxProtectionNumber" "nickserv.MaxProtectionNumber"
find_int "NSMaxNickChange" "nickserv.MaxNickChanges"
find_switch "ForceStrongPasswords" "nickserv.StrongPasswords"
find_int "NSExpire" "nickserv.ExpireTime"
find_int "NickPassExpire" "nickserv.PassExpireTime"
find_string "NSRoot" "nickserv.Root"
find_string "PhotoBaseUrl" "ns_photo.BaseURL"
find_string "CS_Nick" "chanserv.Nick"
find_string "CS_Username" "chanserv.Username"
find_string "CS_Host" "chanserv.Hostname"
find_string "CS_Info" "chanserv.Realname"
find_string "CSLogChan" "chanserv.LogChan"
find_int "MaxChansPerUser" "chanserv.MaxChansPerUser"
find_int "CSExpire" "chanserv.ExpireTime"
find_int "MaxChanAkicks" "cs_akick.MaxAkicksPerChan"
find_int "LastRegCount" "cs_lastreg.DisplayCount"
find_int "MaxChanRoles" "cs_role.MaxRolesPerChan"
find_int "MaxChanUsers" "cs_role.MaxUsersPerChan"
find_int "ExpireInterval" "expire.Interval"
find_string "MS_Nick" "memoserv.Nick"
find_string "MS_Username" "memoserv.Username"
find_string "MS_Host" "memoserv.Hostname"
find_string "MS_Info" "memoserv.Realname"
find_int "MaxMemosPerUser" "memoserv.MaxMemosPerUser"
find_string "MSLogChan" "memoserv.LogChan"
find_string "OS_Nick" "operserv.Nick"
find_string "OS_Username" "operserv.Username"
find_string "OS_Host" "operserv.Hostname"
find_string "OS_Info" "operserv.Realname"
find_string "OSLogChan" "operserv.LogChan"
find_string "OperChan" "operserv.OperChan"
find_string "SAdminChan" "operserv.SAdminChan"
find_switch "OperControl" "operserv.OperControl"
find_string "HR_Nick" "os_hostrule.Nick"
find_string "HR_Username" "os_hostrule.Username"
find_string "HR_Host" "os_hostrule.Hostname"
find_string "HR_Info" "os_hostrule.Realname"
find_int "DefSessionLimit" "os_hostrule.DefaultMaxUsers"
find_int "MaxSessionHits" "os_hostrule.MaxUsersHits"
find_int "SessionsGlineTime" "os_hostrule.MaxUsersGlineTime"
find_string "EmailFrom", "email.EmailFrom"
find_string "EmailFromName", "email.EmailFromName"
