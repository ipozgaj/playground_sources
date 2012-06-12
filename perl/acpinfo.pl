#!/usr/bin/perl

use strict;
use warnings;

open (IF, "</proc/acpi/ibm/thermal") or die "No ibm-acpi module found";
my @values = map { split /\s+/ } <IF>;
close(IF);
my %t = (
	"CPU"  => $values[1],	# CPU
	"PCI"  => $values[2],	# Mini-PCI
	"HDD"  => $values[3],	# Internal hard disk
	"BAT1" => $values[5],	# main battery, primary sensor (under F5)
	"BAT2" => $values[7],	# main battery, secondary sensor (under Home)
	"MCH"  => $values[9],	# northbridge, to DRAM bus (above track point)
	"ICH"  => $values[10],	# southbridge (under touchpad)
	"PWR"  => $values[11],	# power regulator (under F2 key)
);


open(IF, "</proc/acpi/ibm/fan") or die "No ibm-acpi module found";
my @line = grep { /speed:/ } <IF>;
my $fan = (split(/:\s+/, $line[0]))[1];
close(IF);

write;

format STDOUT_TOP=
                          Thermal data for IBM
  CPU     PCI     HDD    BAT1    BAT2     MCH     ICH     PWR      FAN
--------------------------------------------------------------------------
.

format STDOUT=
@## °C  @## °C  @## °C  @## °C  @## °C  @## °C  @## °C  @## °C  @#### rpm
$t{"CPU"}, $t{"PCI"}, $t{"HDD"}, $t{"BAT1"}, $t{"BAT2"}, $t{"MCH"}, $t{"ICH"}, $t{"PWR"}, $fan
.
