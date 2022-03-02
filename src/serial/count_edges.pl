#!/usr/bin/perl
#
# read a file of ordered data pairs and count mid-level crossings
#
use strict;

my $dsum = 0;
my $dcount = 0;
my $dmin = 9999;
my $dmax = -9999;
my @pts;

while( my $line = <>) {
    chomp $line;
    if( $line !~ /#/) {
	my @d = split / /, $line;
	$dsum += $d[1];
	$dcount++;
	$dmin = $d[1] if( $d[1] < $dmin);
	$dmax = $d[1] if( $d[1] > $dmax);
	push @pts, $d[1];
    }
}

my $dmean = $dsum / $dcount;
my $dmed = ($dmin + $dmax)/2;
print "# points=$dcount min=$dmin max=$dmax mean=$dmean median=$dmed\n";

my $dlast = 0;
my $d2last = 0;

foreach my $pt ( @pts) {
    my $d = $pt - $dlast;
    my $d2 = $d - $d2last;

    print "$d $d2\n";

    $dlast = $pt;
    $d2last = $d;
}

    
