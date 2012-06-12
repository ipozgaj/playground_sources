#!/usr/bin/perl
# (C) 2009 Igor Pozgaj <igor.pozgaj@s-itsolutions.hr>
# Translate file to correct encoding to be printed on printer

use strict;
use warnings;

my %translation_table = (
	230	=>	134,
	198	=>	143,
	232	=>	159,
	174	=>	166,
	190	=>	167,
	200	=>	172,
	240	=>	208,
	208	=>	209,
	169	=>	230,
	185	=>	231,
	10	=>	13
);

my %printer_options = (
	"croascii" 	=>	"(17U", 
	"landscape"	=>	"&l1O", 
	"portrait"	=>	"&l0O", 
	"A4"		=>	"&l26A", 
	"line6"		=>	"&l6D", 
	"line8"		=>	"&l8D", 
	"line12"	=>	"&l12D", 
	"line16"	=>	"&l16D", 
	"line7"		=>	"&l7C", 
	"pitch10"	=>	"&k0S", 
	"pitch12"	=>	"&k4S", 
	"pitch16"	=>	"&k2S", 
	"HMI5"		=>	"&k5H", 
	"HMI6"		=>	"&k6H", 
	"HMI7"		=>	"&k7H", 
	"HMI8"		=>	"&k8H", 
	"HMI9"		=>	"&k9H", 
	"HMI10"		=>	"&k10H", 
	"HMI11"		=>	"&k11H", 
	"HMI12"		=>	"&k12H"
);

# initialize printer
print chr(27) . chr(69);

# set printer options
foreach my $option (qw/croascii A4 line7 pitch10 HMI11/)
{
	print chr(27);
	print $printer_options{$option};
}

my ($prevch, $currch);

# process input, translate characters if neccessary
while (<>) {
	for my $ch (split //, $_) {
		if (defined($translation_table{ord($ch)})) {
			print chr($translation_table{ord($ch)})
		} else {
			print $ch
		}

		print $ch if ord($ch) == 10;
		
		$prevch = $currch;
		$currch = $ch;
	}
}

# print FF if neccessary (iif file does not end with FF or LF FF)
print chr(12) unless ($currch eq chr(12) || ($currch eq chr(10) && $prevch eq chr(12)))
