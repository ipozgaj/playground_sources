#!/usr/bin/perl

$port = 22;

while (1) {
	do {
		$f1 = int (rand (222)) + 1;
	} until ($f1 != 127 && $f1 != 192);

	$f2 = int (rand (254)) + 1;
	$f3 = int (rand (254)) + 1;
	$f4 = int (rand (254)) + 1;

	$ip =  $f1 . "." . $f2 . "." . $f3 . "." . $f4;

	print $ip . "\t" . " ==> ";
	print `nmap -sS -p$port -P0 $ip 2>&1 | grep $port | grep tcp`;
}
