#!/usr/bin/perl -w
#
# Regina - A Normal Surface Theory Calculator
# Source Code Statistics
#
# Copyright (c) 2003-2025, Ben Burton
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
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
use strict;

if (not (-e 'LICENSE.txt' and -d 'engine')) {
    print STDERR "ERROR: You must be in Regina's root directory.\n";
    exit 1;
}

my @ext = qw(cpp cc c h hpp tcc m mm py swift test);
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
my $pybind11 = `(find python/pybind11_v2 $types; find python/pybind11_v3 $types) | xargs cat | wc -l`;
my $docstrings = `find python/docstrings $types | xargs cat | wc -l`;
my $gtest = `find engine/testsuite/gtest $types | xargs cat | wc -l`;

# Convert strings to integers:
$all = $all + 0;
$snappea = $snappea + 0;
$snappy = $snappy + 0;
$normaliz = $normaliz + 0;
$pybind11 = $pybind11 + 0;
$docstrings = $docstrings + 0;
$gtest = $gtest + 0;

my $regina = $all - $snappea - $snappy - $normaliz - $pybind11 - $docstrings - $gtest;

print "Gross lines:  $all\n";
print "- Regina:     $regina\n";
print "- SnapPea:    $snappea\n";
print "- SnapPy:     $snappy\n";
print "- Normaliz:   $normaliz\n";
print "- pybind11:   $pybind11\n";
print "- Docstrings: $docstrings\n";
print "- GoogleTest: $gtest\n";
print "\n";

my $test_engine = `find engine/testsuite $types | xargs cat | wc -l` + 0;
my $test_python = `find python/testsuite $types | xargs cat | wc -l` + 0;
my $test_utils = `find utils/testsuite $types | xargs cat | wc -l` + 0;
$test_engine -= $gtest;
my $test_all = $test_engine + $test_python + $test_utils;

print "Test suite:   $test_all  (excludes GoogleTest)\n";
print "- Engine:     $test_engine\n";
print "- Python:     $test_python\n";
print "- Utils:      $test_utils\n";

exit 0;
