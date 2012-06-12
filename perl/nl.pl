#!/usr/bin/perl
# numerates lines in file
# (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

while (<>) {
	print $., " ", $_;
}
