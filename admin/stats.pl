#!/usr/bin/perl -w
#
# Regina - A Normal Surface Theory Calculator
# Source Code Statistics
#
# Copyright (c) 2003-2023, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# Usage: stats.pl
#
# Gives some very basic summary statistics about Regina's source code.
#
# This script must be run from the top-level source directory within
# the git source tree.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
# MA 02110-1301, USA.
#
use strict;

if (not (-e 'LICENSE.txt' and -d 'engine')) {
    print STDERR "ERROR: You must be in Regina's root directory.\n";
    exit 1;
}

my @ext = qw(cpp cc c h hpp tcc m mm py);
my $types = '\\( -name "*.' . join('" -o -name "*.', @ext) . '" \\)';

# Find any builddirs that should be ignored.
my @builddirs = ();
opendir(my $dh, '.') or die 'Could not open ./';
while (readdir $dh) {
    ($_ eq '.' or $_ eq '..') and next;
    -d $dh and -e "$_/CMakeCache.txt" and push @builddirs, $_;
}
closedir $dh;

my $exclusions;
if (@builddirs) {
    print "Excluding build directories: @builddirs\n";
    foreach (@builddirs) {
        /'/ and die "Build directory name contains special characters";
        if (not $exclusions) {
            $exclusions = "\\( \\! \\( -path './$_/*'";
        } else {
            $exclusions .= " -o -path './$_/*'";
        }
    }
    $exclusions .= ' \\) \\)';
} else {
    print "No build directories to exclude\n";
    $exclusions = '';
}

my $all = `find . $types $exclusions | xargs cat | wc -l`;
my $snappea = `find engine/snappea/kernel $types | xargs cat | wc -l`;
my $snappy = `find engine/snappea/snappy $types | xargs cat | wc -l`;
my $normaliz = `find engine/libnormaliz $types | xargs cat | wc -l`;
my $pybind11 = `find python/pybind11 $types | xargs cat | wc -l`;

# Convert strings to integers:
$all = $all + 0;
$snappea = $snappea + 0;
$snappy = $snappy + 0;
$normaliz = $normaliz + 0;
$pybind11 = $pybind11 + 0;

my $regina = $all - $snappea - $snappy - $normaliz - $pybind11;

print "Gross lines: $all\n";
print "- Regina:    $regina\n";
print "- SnapPea:   $snappea\n";
print "- SnapPy:    $snappy\n";
print "- Normaliz:  $normaliz\n";
print "- pybind11:  $pybind11\n";
exit 0;
