#!/usr/bin/perl

use strict;
use warnings;

my ($host, $ip, $min, $max, $avg);
my ($min_total, $max_total, $avg_total) = (0, 0, 0);

my @hosts = (
	"google.com",
	"google.hr",
	"fer.hr",
	"rapidshare.de",
	"nasa.gov"
);

my $num_of_hosts = $#hosts;

format STDOUT_TOP =
         Host                IP           Min     Max     Avg
    -----------------  ---------------   -----   -----   -----
.

format STDOUT = 
    @<<<<<<<<<<<<<<<<  @<<<<<<<<<<<<<< @####.# @####.# @####.#
	$host,             $ip,            $min,   $max,   $avg
.

format STDOUT_TOTAL =
    ----------------------------------------------------------
    Total:                             @####.# @####.# @####.#
                                       $min_total, $max_total, $avg_total
.


for $host (@hosts) {
	my @data = split(/\n/, `ping -c 4 $host`);
	$data[0] =~ /\((.*)\)/;
	$ip = $1;
	$data[$#data] =~ /=\ (.*)\/(.*)\/(.*)\ ms/;
	($min, $avg, $max) = ($1, $2, $3);
	$min_total += $min;
	$max_total += $max;
	$avg_total += $avg;
	write;
}

$avg_total /= $num_of_hosts;
$~ = "STDOUT_TOTAL";

write;
