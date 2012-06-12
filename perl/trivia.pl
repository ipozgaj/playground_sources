# Trivia answerer bot version 0.1
# (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
#
# NOTE: 
# Please use this in experimental purposes only. Scripts ruin the game 
# and you will probably get banned if you get discovered running one.

use Irssi;
use strict;
use vars qw ($VERSION %IRSSI);

$VERSION = '0.1';

%IRSSI = (
	authors		=> 'Igor Pozgaj',
	name		=> 'trivia',
	description	=> 'Trivia quiz bot',
	licence 	=> 'GPL2',
	changed 	=> 'Tue Jul 5 17:24 CET 2005'
);

my $read_speed = 0.05;
my $write_speed = 0.2;
my $botname = 'ZecBot';
my $verbose = 0;
my $writemode = 0;

my %answers;
my $question;

sub event_public_message ($$$$$) {
	my ($server, $text, $nick, $address, $target) = @_;

	$text =~ s/^\s+|\x03\d?\d?(,\d?\d?)?|\x02|\x1f|\x16|\x06|\x07|\x0f//g;

	return if $text =~ /hint/ || $nick !~ /$botname/;

	if ($text !~ "Odgovor" && $text =~ /\d+/ && $text !~ "bod" && $text !~ "score" && length ($text) > 20) {
		$question = $&;
		
		if ($answers{$question}) {
			if ($writemode) {
				my $sleep_interval;

				$sleep_interval += length ($text) * $read_speed;
				$sleep_interval += length ($answers{$question}) * $write_speed;
			
				select (undef, undef, undef, $sleep_interval);
				send_msg ($target, "\L$answers{$question}\e");
			} else {
				Irssi::active_win->print ("\L$answers{$question}\e");
			}
		} else {
			Irssi::print "==> No such question in database <==" if $verbose;
		}
	} elsif ($text =~ /Odgovor/) {
		unless ($question eq "") {
			my $tmp;
			
			$text =~ /->(.+)<-/;
			$tmp = $&;
			$tmp =~ s/-|>|<//g;
			$tmp =~ s/^\s+//g;
			$tmp =~ s/\s+$//g;
			
			$answers{$question} = $tmp;
			Irssi::print "==> Added new answer to database <==" if $verbose;
		}
	}
}

sub command_bot {
	my ($data, $server, $channel) = @_;

	if ($data eq "save") {
		save_answers ($botname);
	} elsif ($data eq "load") {
		load_answers ($botname);
	} elsif ($data eq "quiet") {
		$verbose = 0;
		Irssi::print ("Bot verbosity: off");
	} elsif ($data eq "verbose") {
		$verbose = 1;
		Irssi::print ("Bot verbosity: on");
	} elsif ($data eq "write") {
		$writemode = 1;
	} elsif ($data eq "nowrite") {
		$writemode = 0;
	} else {
		Irssi::print ("Unknown bot command: " . $data); 
	}
}

sub save_answers ($) {
	my $key;
	my $i = 0;
	my $filename = shift;

	open (OUT, ">$filename") or die "Cannot open answers database file: $!";

	for $key (keys %answers) {
		print OUT "$key#$answers{$key}\n";
		++$i;
	}
	
	close (OUT);

	Irssi::print "==> Answers database saved ($i answers) <==";
}

sub load_answers ($) {
	my $i = 0;
	my $filename = shift;

	open (IN, "$filename") or die "Cannot open answers database file: $!";

	while (<IN>) {
		chomp;
		my ($key, $value) = split (/#/);
		$answers{$key} = $value;
		++$i;
	}

	Irssi::print "==> Loaded answers database ($i answers) <==";

	close (IN);
}

sub send_msg ($$) {
	my ($target, $msg) = @_;
	my $witem = Irssi::window_item_find ($target);
	
	$witem->{server}->command ("MSG $target $msg");
}

Irssi::print ('=' x 55);
Irssi::print ('                TRIVIA QUIZ BOT LOADED');
Irssi::print ('    (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>');
Irssi::print ('NOTE: You will probably be banned if you get discovered');
Irssi::print ('=' x 55);

Irssi::signal_add ('message public', 'event_public_message');
Irssi::command_bind ('bot', 'command_bot');
