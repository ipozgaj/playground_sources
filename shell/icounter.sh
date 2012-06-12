#!/bin/sh
# calculates time on the Internet
# (C) 2002 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

syslog="/var/log/syslog"

grep "Connect time" $syslog | cut -d: -f4 | cut -d" " -f4 | awk '{sum += $1} END{print sum}'
