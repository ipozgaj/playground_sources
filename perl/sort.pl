#!/usr/bin/perl
# (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

@words = <STDIN>;

for $i (sort @words) {
	print $i;
}
