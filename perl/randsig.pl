#!/usr/bin/perl
# Dodaje nasumicno odabrani potpis iz datoteke $quote_file
# u sve datoteke u direktoriju $spool
# Potpisi su u datoteci odvojeni delimiterom $delimiter
# 
# (C) Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
# Zadnja izmjena: 2.7.2003.

$quotes_file = "./quotes";
$delimiter   = "%%";
$spool_dir   = "./spool";

$quotes_count = `grep $delimiter $quotes_file | wc -l`;
@spool_files=`ls $spool_dir`;

open (in_file, $quotes_file) || die "\nCould not open quotes file\n";

foreach $file (@spool_files) {
	seek (in_file, 0, 0);
	$n=0;
	
	$rand_quote_n = int(int($quotes_count)*rand)+1;

	while ($n < $rand_quote_n) {
		$input = <in_file>;
		chop $input;

		if ($input eq "%%") {
			$n++;
		}
	}

	open (out_file, ">>".$spool_dir."/".$file) || die "\nOutput file error\n";
	
	# File pointer now points on the quote first line
	# First add an empty line and -- characters
	
	print out_file "\n--\n";

	do {
		$input = <in_file>;
		chop $input;
	
		if ($input ne "%%") {
			print out_file $input."\n";
		}
	} until ($input eq "%%")
}
