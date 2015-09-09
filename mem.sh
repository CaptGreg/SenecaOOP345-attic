#!/bin/bash 
# Written by Alexander Löhner 
# The Linux Counter Project 
# Updated on Sep. 8th 2015 by Mike Hay to remove dependency on 'bc' 
 

# http://blog.linuxcounter.net/2015/09/05/bash-collection-script-to-get-the-real-amount-of-used-memory-of-an-application/

# Bash Collection – Script to get the real amount of used memory of an application
# 
# The following script was written by me to be able to get the really used amount of memory for one single application.
# Since it isn’t that easy to get the used memory through “top” or “ps” or similar tools, this may be helpful for somebody.
# 
# The output is in megabytes and looks like this:
# 
# ~ $ memused.sh mono
# 5438.39


if [[ "${1}" = "" ]]; then
        echo "please add the name of the process as a parameter"
        exit 1
fi

for i in $(pidof $1); do echo $(awk '/Private/ {sum+=$2} END {print sum}' /proc/$i/smaps); done | awk '{sum+=$1} END {printf("%.2f MBytes\n", sum/1000)}'
