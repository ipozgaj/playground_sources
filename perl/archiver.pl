#!/usr/bin/perl

use strict;
use warnings;

use File::Copy;
use File::stat;
use POSIX qw(strftime);

sub arrange_files($$) 
{
	my ($basedir, $archdir) = @_;

	return if ( ! -d $basedir );

	foreach (glob("$basedir/*")) {
		next if ! -f;
		my ($filename, $fileage) = ($_, -M);

		if ($fileage > 4) {
			my $dd = strftime "%d", localtime(stat($_)->mtime);
			my $mm = strftime "%m", localtime(stat($_)->mtime);	
			my $yyyy = strftime "%Y", localtime(stat($_)->mtime);

			mkdir $archdir if not -d $archdir;
			chdir $archdir;

			mkdir $yyyy if not -d $yyyy;
			mkdir "$yyyy/$mm" if not -d "$yyyy/$mm";
			mkdir "$yyyy/$mm/$dd" if not -d "$yyyy/$mm/$dd";

			move("$filename", "$archdir/$yyyy/$mm/$dd");
		}
	}
}

arrange_files("/d01/prints", "/d01/prints/archive");
arrange_files("/d01/logs", "/d01/logs/archive");

arrange_files("/d02/prints", "/d02/prints/archive");
arrange_files("/d02/logs", "/d02/logs/archive");

arrange_files("/d03/prints", "/d03/prints/archive");
arrange_files("/d03/logs", "/d03/logs/archive");

arrange_files("/d04/prints", "/d04/prints/archive");
arrange_files("/d04/logs", "/d04/logs/archive");
