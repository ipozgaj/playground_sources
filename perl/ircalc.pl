use Irssi;
use strict;
use vars qw ($VERSION %IRSSI);

$VERSION = '0.1';
%IRSSI = (
	authors => 'Igor Pozgaj',
	contact => 'ipozgaj@fly.srk.fer.hr',
	name => 'calc',
	description => 'calculator',
	licence => 'gpl',
	changed => 'Wed Jul 6 11:43 CET 2005'
);

sub event_public_msg ($$$$$) {
	my ($server, $text, $nick, $address, $target) = @_;
	
	return if $text !~ /calc/ or $text =~ /`/;
	
	$text =~ /calc\s*\((.+)\)/;

	(my $res = $&) =~ s/calc//g;
	$res = eval ($res);
	
	Irssi::active_win->command ("say $nick: Rezultat = $res");
}

Irssi::signal_add ('message public', 'event_public_msg');
