#!/usr/bin/perl

=pod

=head1 NAME

litrain.pl - script for training dictionary knowledge

=head1 SYNOPSIS

B<litrain.pl> [OPTION] I<dictionary-file>

=head1 DESCRIPTION

The script works by displaying a word and asking the user if the word exists in the specified dictionary. Some of the words are altered (i.e. they have one or more of their letters exchanged for another letter), so that the user has to guess which word has been taken straight from the dictionary and which one was altered.

The user responds YES by pressing B<y>, B<t> or B<1> and NO by pressing any other key. The script displays information on whether the guess was correct and proceeds to another word. Break the loop by interrupting the script (Ctrl-C).

=head2 PARAMETERS

=over 

=item I<dictionary-file> 

This can be either the name of the dictionary file or the processed output file from a previous script run with B<-o>. The dictionary file should be encoded in UTF-8.

=back

=head2 OPTIONS

=over

=item B<--output>, B<-o> I<file>

Write processed dictionary to I<file>. This allows for subsequent runs of the script to start significantly faster than if you read the dictionary from scratch. Please note that the output file formats may not be compatible between versions of the script.

=item B<--max-length>, B<-l> I<num>

Only ask for words which are not longer than I<num> letters. With B<-o>, only store such words in the processed output.

=item B<--max-alternations>, B<-a> I<num>

Introduce no more than I<num> letter alternations to any given word.

=back

=head1 REQUIREMENTS

The script requires the following non-standard Perl modules to be installed:

=over

=item * I<Term::ReadKey>

=back

=head1 FUTURE CHANGES

=over

=item * Fetching word definitions from the on-line dictionary.

=item * Gathering statistics on the user's progress.

=item * Remembering words for which the user has erred, so that the script may repeat asking for such words (like SuperMemo).

=back

=cut

use strict; 
use warnings;
use utf8;
use open qw/:std :utf8/;

use Getopt::Long;
use Term::ReadKey;
use Data::Dumper;
use Storable;
use Pod::Usage;

my $max_length = undef;
my $max_alternations = 1;
my $output_filename = undef;

my $help = 0;
my $man = 0;

GetOptions(
    'max-length|l:i' => \$max_length,
    'max-alternations|a:i' => \$max_alternations,
    'output|o:s' => \$output_filename,

    'help|h' => \$help,
    'man|m' => \$man,
) or pod2usage();

pod2usage(verbose => 2) if $man;

my ($dict_filename) = @ARGV;

my @glyphs = qw/a ą b c ć d e ę f g h i j k l ł m n ń o ó p r s ś t u w y z ź ż/;
my @vowels = qw/a ą e ę i o ó u y/;
my %vowels = map { $_ => 1 } @vowels;
my @consonants = qw/b c ć d f g h j k l ł m n ń p r s ś t w z ź ż/;

# The triglyphs hash stores an entry for each three-letter combination
# occurring in the dictionary. Pound signs (#) are used to mark beginning
# and end of a word.
my $triglyphs = {};
my $words = {};
my $n = 0;

my $data = eval { retrieve $dict_filename };
if (!defined $data) {
    open my $dict_fh, '<', $dict_filename or die "Failed to open $dict_filename: $!\n";
    while (<$dict_fh>) {
	chomp;
	s/\r//;
	next if defined $max_length && length $_ > $max_length;
	my $l = length $_;
	my $i = 0;
	my @tri = ();
	my $tri_word = "#$_#";
	while ($tri_word =~ m/(...)/g) {
	    $triglyphs->{$1}++;
	    pos $tri_word = ++$i;
	}
	$words->{$_} = 1;
	$n++;

	print STDERR "$n\r" if -t STDERR && $n % 1000 == 0;
    }
    print STDERR "\n" if -t STDERR;
    close $dict_fh;

    store { triglyphs => $triglyphs, words => $words, n => $n }, $output_filename if defined $output_filename;
} else {
    $triglyphs = $data->{triglyphs};
    $words = $data->{words};
    $n = $data->{n};
}

my @words = keys %$words;

while (1) {
    my $rand_idx = int rand($n);
    my $selected_word = $words[$rand_idx];
    my $ask_word = $selected_word;

    next if defined $max_length && length $ask_word > $max_length;

    if (rand() < 0.5) {
	$ask_word = &alter_word($selected_word);
    }

    print "'$ask_word'";
    ReadMode('cbreak');
    my $answer = ReadKey();
    ReadMode('normal');

    my $answer_yes = ($answer =~ m/[yt1]/i);
    my $word_exists = defined $words->{$ask_word};

    if (($answer_yes && $word_exists) || (!$answer_yes && !$word_exists)) {
	print " $answer [OK]\n";
    } else {
	print " $answer [WRONG!]\n";
    }
}

sub alter_word() {
    my ($word) = @_;

    my $alt_count = int(rand($max_alternations || 1)) + 1;
    my @letters = split //, $word;
    while ($alt_count) {
	my $li = int rand(int @letters);
	while (1) {
	    my $g = $vowels{$letters[$li]} ? $vowels[int rand(int @vowels)] : $consonants[int rand(int @consonants)];
	    my $trig = join '', ($li > 0 ? $letters[$li-1] : '#', $g, $li < $#letters ? $letters[$li+1] : '#');
	    if ($triglyphs->{$trig}) {
		$letters[$li] = $g;
		last;
	    };
	}

        $alt_count--;
    }

    return join '', @letters;
}

