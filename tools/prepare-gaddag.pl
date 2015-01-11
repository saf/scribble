#!/usr/bin/perl 

use strict;
use warnings;
use utf8;
use open qw/:std :utf8/;

use Data::Dumper;

warn Dumper("cat", reverse("cat"));


my $entries = {};
my $i = 0;
while (my $word = <>) {
	chomp $word;

	my $len = length $word;
	for my $i (0..$len) {
		printf "%s#%s\n", scalar(reverse(substr($word, 0, $i))), substr($word, $i);
	}

	$i++;
	printf STDERR "%d\r", $i if -t STDERR && $i % 1000 == 0;
}
printf STDERR "\n" if -t STDERR;


