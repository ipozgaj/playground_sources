#!/bin/sh
# Displays dropped input packets destination port statistics

# Identifier in logs
STR="IPT INPUT"

# default log file
LOG='/var/log/syslog'

grep "$STR" $LOG | tr ' ' '\n' | grep DPT | sort | uniq -c | sort -n -r | head -10
