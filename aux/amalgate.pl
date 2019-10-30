#!/usr/bin/env perl

use strict;
use warnings;
use File::Spec;

my $defines = {};

sub slurpfile {
    my $filename = shift;
    open(my $fh, '<', $filename) or die "File $filename: $!";
    my @lines = <$fh>;
    close($fh);
    return @lines;
}

sub processfile {
    my $filename = shift;
    my @lines = @_;
    my $output = '';

    # check of ifndef-guard
    my ($def1) = ( $lines[0] =~ /^#ifndef\s+(.+)$/);
    if(defined($def1)) {
        chomp($def1);
        my ($def2) = ( $lines[1] =~ m/^#define\s+(.+)$/);
        if(defined($def2)) {
            chomp($def2);

            if($def1 eq $def2) {
                if(exists($defines->{$def1})) {
                    return '';
                }
                else {
                    $defines->{$def1} = 1;
                }
            }
        }
    }

    foreach my $line (@lines) {
        $output .= processline($filename,$line);
    }

    return $output;
}

sub processline {
    my $filename = shift;
    my $line = shift;

    if($line !~ /^#include\s+"/) {
        return $line;
    }

    my ($volume, $dir, $file) = File::Spec->splitpath($filename);
    $dir = $volume . $dir;
    my ($newfile) = ( $line =~ /^#include\s+"([^"]+)"/);
    $newfile = File::Spec->catfile($dir,$newfile);

    my @lines = slurpfile($newfile);
    return processfile($newfile,@lines);
}

if(@ARGV < 1) {
    print STDERR "Usage: amalage.pl /path/to/source.c\n";
    exit(1);
}

my $output = "namespace {\n";

foreach my $file (@ARGV) {
  my @lines = slurpfile($file);
  $output .= processfile($file,@lines);
}

$output .= "}\n";

print $output;
