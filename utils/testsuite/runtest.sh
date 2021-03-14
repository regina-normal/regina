#!/bin/bash
#
# Regina - Software for low-dimensional topology
# Command-Line Utilities Test Suite
#
# Copyright (c) 2016-2021, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# This script runs all tests in a single *.test file.
#
# Usage: runtest.sh <test_file> [<filter>]
#
# Each line of <test_file> should be of the form:
#
#  utility argument argument ...
#
# Utilities must be the utility name only (e.g., censuslookup, not
# path/to/censuslookup or censuslookup.exe).
#
# Arguments must not contain whitespace or bash special characters.
# Any arguments of the form TESTOUT, INVALIDFILE or BADFILE will be
# replaced with the corresponding environment variables (see below).
#
# Lines beginning with # will be ignored.
#
# If <filter> is given, the output from each line will be piped through it.
# Note that this script provides an additional filter that is always run, which
# replaces specific paths/files from the environment variables below with
# generic "paths" and "files" that can be matched against the expected output.
#
# Optional environment variables that can be set when running this script:
#
# - testdir : the directory containing the test input files
#               (default: location of this script)
# - bindir : the directory containing the compiled command-line utilities
#               (default: "$testdir/..")
# - testout : a suitable output file if one is required
#               (default: "$TMPDIR/test.rga")
# - invalidfile : a file that you can neither read from nor write to
#               (default: /foo.rga)
# - badfile : a file that exists but is not a Regina data file
#               (default: "$testdir/bad.rga")
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

set -e
set -o pipefail

if [ -z "$testdir" ]; then
    testdir="`dirname "$0"`"
fi
if [ ! -e "$testdir"/test.rga ]; then
    echo "ERROR: Test directory "$testdir" does not contain test.rga."
    exit 1
fi

if [ -z "$bindir" ]; then
    bindir="$testdir/.."
fi
if [ ! -e "$bindir"/regfiledump -o -e "$bindir"/regfiledump.exe ]; then
    echo "ERROR: Binary directory "$bindir" does not contain regfiledump."
    exit 1
fi

if [ -z "$testout" ]; then
    testout="$TMPDIR/test.rga"
fi
if ! touch "$testout"; then
    echo "ERROR: Output file $testout cannot be written to."
    exit 1
fi
if ! rm -f "$testout"; then
    echo "ERROR: Output file $testout cannot be deleted."
    exit 1
fi

if [ -z "$invalidfile" ]; then
    invalidfile=/foo.rga
fi
if [ -e "$invalidfile" ]; then
    echo "ERROR: Invalid file $invalidfile exists."
    exit 1
fi
if touch "$invalidfile" 2>/dev/null; then
    echo "ERROR: Invalid file $invalidfile can be written to."
    exit 1
fi

if [ -z "$badfile" ]; then
    badfile="$testdir/bad.rga"
fi
if [ ! -e "$badfile" ]; then
    echo "ERROR: Bad regina file $badfile does not exist."
    exit 1
fi

input="$1"
if [ ! -e "$input" ]; then
    echo "ERROR: Test file $input does not exist."
    exit 1
fi

filter="$2"

function pathfilter {
    _from="$bindir/$util" _to="$util" perl -pe 's/\Q$ENV{_from}\E/$ENV{_to}/g'
    # TODO: Fix TESTOUT, INVALIDFILE, BADFILE
}

while read -r -a line; do
    util="${line[0]}"
    args=( "${line[@]:1}" )
    # TODO: Replace TESTOUT, INVALIDFILE, BADFILE
    case "$util" in
        '' )
            ;;
        '#'* )
            ;;
        * )
            echo "============================================================"
            echo "TEST: $util ${line[@]:1}"
            echo "--------------------"
            rm -f "$testout"
            if [ -z "$filter" ]; then
                "$bindir/$util" "${line[@]:1}" 2>&1 | pathfilter && dummy=
                exitcode=$?
            else
                "$bindir/$util" "${line[@]:1}" 2>&1 | pathfilter | "$filter" && dummy=
                exitcode=$?
            fi
            echo "--------------------"
            echo "Exit code: $exitcode"
            if [ -e "$testout" ]; then
                echo "--------------------"
                raw=`cat "$testout" | shasum`
                uncompressed=`zcat -f "$testout" | shasum`
                if [ "$raw" = "$uncompressed" ]; then
                    echo "Output:"
                    if [ -z "$filter" ]; then
                        cat "$testout"
                    else
                        cat "$testout" | "$filter"
                    fi
                else
                    echo "Output (compressed):"
                    if [ -z "$filter" ]; then
                        zcat "$testout"
                    else
                        zcat "$testout" | "$filter"
                    fi
                fi
                rm "$testout"
            fi
            ;;
    esac
done < "$input"
echo "============================================================"

