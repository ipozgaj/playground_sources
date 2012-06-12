#!/usr/bin/perl

$s = "";
foreach (split //, "$ARGV[0]\n") {$s .= sprintf "%08b", ord};
print  `echo "ibase=2;$s" | bc`;
