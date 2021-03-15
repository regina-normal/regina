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
# Any instances of @TESTDIR@, @TESTOUT@, @INVALIDFILE@ or @BADFILE@ in the
# arguments will be replaced with the corresponding environment variables
# (see below), or their defaults if the environment variables are not set.
#
# There is very limited capacity for handling arguments with whitespace:
# such arguments may be surrounded by single quotes, though each block of
# whitespace will be compressed to a single space.  An empty argument
# may be specified with a pair of single quotes ('').  Currently there is
# no way to escape single quotes within an argument.
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
#               (default: /foo/foo.rga)
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
    invalidfile=/foo/foo.rga
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

# Ensure that our perl substitution scripts can see all of these variables.
export testdir bindir testout invalidfile badfile

input="$1"
if [ ! -e "$input" ]; then
    echo "ERROR: Test file $input does not exist."
    exit 1
fi

filter="$2"

function pathfilter {
    _from="$bindir/$util" _to="$util" \
        perl -pe 's/\Q$ENV{_from}\E/$ENV{_to}/g;
                  s/\Q$ENV{invalidfile}\E/\@INVALIDFILE\@/g;
                  s/\Q$ENV{badfile}\E/\@BADFILE\@/g;
                  s/\Q$ENV{testout}\E/\@TESTOUT\@/g;
                  s/\Q$ENV{testdir}\E/\@TESTDIR\@/g';
}

while read -r -a line; do
    util="${line[0]}"
    case "$util" in
        '' ) continue ;;
        '#'* ) continue ;;
    esac

    args=()
    next=
    waiting=0
    for arg in "${line[@]:1}"; do
        if [ "$waiting" = 0 ]; then
            # We are beginning a new argument.
            case "$arg" in
                "'"*"'" )
                    next="`printf '%s\n' "$arg" | sed -e 's#^.##' -e 's#.$##'`"
                    ;;
                "'"* )
                    next="`printf '%s\n' "$arg" | sed -e 's#^.##'`"
                    waiting=1
                    ;;
                * )
                    next="$arg"
                    ;;
            esac
        else
            # We are continuing a quoted argument.
            case "$arg" in
                *"'" )
                    next="$next `printf '%s\n' "$arg" | sed -e 's#.$##'`"
                    waiting=0
                    ;;
                * )
                    next="$next $arg"
                    ;;
            esac
        fi
        if [ "$waiting" = 0 ]; then
            args+=("`printf '%s\n' "$next" | \
                perl -pe 's/\@TESTDIR\@/$ENV{testdir}/g;
                          s/\@TESTOUT\@/$ENV{testout}/g;
                          s/\@INVALIDFILE\@/$ENV{invalidfile}/g;
                          s/\@BADFILE\@/$ENV{badfile}/g;'`")
            next=
        fi
    done
    if [ "$waiting" = 1 ]; then
        echo "ERROR: Unfinished quoted argument: $next"
        exit 1
    fi

    echo "============================================================"
    echo "TEST: ${line[@]}"
    echo "--------------------"
    rm -f "$testout"
    if [ -z "$filter" ]; then
        "$bindir/$util" "${args[@]}" 2>&1 | pathfilter && dummy=
        exitcode=$?
    else
        "$bindir/$util" "${args[@]}" 2>&1 | pathfilter | "$filter" && dummy=
        exitcode=$?
    fi
    echo "--------------------"
    echo "Exit code: $exitcode"
    if [ -e "$testout" ]; then
        echo "--------------------"
        magic="`hexdump -n2 -e '/2 "%x\n"' "$testout"`"
        if [ "$magic" = 8b1f ]; then
            echo "Output (compressed):"
            if [ -z "$filter" ]; then
                cat "$testout" | gunzip
            else
                cat "$testout" | gunzip | "$filter"
            fi
        else
            echo "Output:"
            if [ -z "$filter" ]; then
                cat "$testout"
            else
                cat "$testout" | "$filter"
            fi
        fi
        rm "$testout"
    fi
done < "$input"
echo "============================================================"

