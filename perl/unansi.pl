#!/usr/bin/perl
# Removes ansi terminal color codes (usefull for clearing BitchX logs)
#
# (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

# while (<>) { s/\x2?\x3\d+|\xf?\x2?//g; print }

while (<>) { s/\x1b\[(\d+;)?\d+m|\x2//g; print }
