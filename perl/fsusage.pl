#!/usr/bin/perl

use strict;
use warnings;

my ($total, $free, $used, $name, $usage, $full);

format STDOUT_TOP = 
Mounted   Used   Total  Full  Usage
======= ======  ======  ====  0======================50=====================100
.

format STDOUT =
@<<<<< @####.# @####.# @##.#  @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
$name, $used,  $total, $full, $usage
.

foreach (`df -g`)
{
	my @data = split /\s+/;
	($total, $free, $name) = @data[1,2,6];

	next if $data[0] !~ /^\// or $data[0] =~ /\/proc/;
	$used = $total - $free;
	$full = $used/$total*100;
	$usage = "*" x ($full/2);
	write;
}
