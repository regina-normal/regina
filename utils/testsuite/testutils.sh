#!/bin/bash
#
# Regina - Software for low-dimensional topology
# Command-Line Utilities Test Suite
#
# Copyright (c) 2016, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# Ad-hoc usage only (for now)
#
# Run this script in the directory containing the executable
# command-line utilities, and compare the output (both stdout and stderr)
# to a known good case.  The correct output is not stored here in the
# repository, since machine-specific paths appear in the output.
#
# Important caveats:
#
# - This makes some possibly dangerous assumptions about hard-coded paths
#   (e.g., that you cannot write to /foo.rga, or that /tmp/out.rga is
#   fair game to create, edit and delete automatically.)
#
# - This has only been tested on MacOS, and may (or may not) rely on
#   features specific to that operating system.  (The main culprit, if any,
#   is likely to be "zcat -f".)
#
# For these reasons, this script is (for now) intended for developer use only.
#
# ----------------------------------------------------------------------
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

# The directory containing the test input files:
testdir=`dirname "$0"`
if [ ! -e "$testdir"/test.rga ]; then
    echo "ERROR: Test directory "$testdir" does not contain test.rga."
    exit 1
fi

# A suitable output file if one is required:
testout=/tmp/out.rga
if ! touch "$testout"; then
    echo "ERROR: Output file $testout cannot be written to."
    exit 1
fi
if ! rm -f "$testout"; then
    echo "ERROR: Output file $testout cannot be deleted."
    exit 1
fi

# A file that you can neither read from nor write to:
invalidfile=/foo.rga
if [ -e "$testout" ]; then
    echo "ERROR: Invalid file $invalidfile exists."
    exit 1
fi
if touch "$invalidfile" 2>/dev/null; then
    echo "ERROR: Invalid file $testout can be written to."
    exit 1
fi

# A file that exists, but is not a Regina data file:
badfile="$testdir"/bad.rga
if [ ! -e "$badfile" ]; then
    echo "ERROR: Bad regina file $badfile does not exist."
    exit 1
fi

# The utilities being tested:
utils="censuslookup regconcat regconvert regfiledump regfiletype trisetcmp"
for i in $utils; do
    if [ ! -x $i ]; then
        echo "ERROR: Could not find the utility $i in the current directory."
        echo
        echo "This tool should be run from the directory containing the executable"
        echo "command-line utilities."
        exit 1
    fi
done

function testutil {
    util="$1"

    skip=1
    for i in $utils; do
        if [ "$i" = "$util" ]; then
            skip=
        fi
    done
    if [ -n "$skip" ]; then
        echo "============================================================"
        echo "Skipping $util (utility not listed)"
        return
    fi

    shift
    echo "============================================================"
    echo "TEST: $util $@"
    echo "--------------------"
    rm -f "$testout"
    ./$util "$@" && dummy=
    exitcode=$?
    echo "--------------------"
    echo "Exit code: $exitcode"
    if [ -e "$testout" ]; then
        echo "--------------------"
        raw=`cat "$testout" | md5sum`
        uncompressed=`zcat -f "$testout" | md5sum`
        if [ "$raw" = "$uncompressed" ]; then
            echo "Output:"
            cat "$testout"
        else
            echo "Output (compressed):"
            zcat "$testout"
        fi
    fi
}

function donetests {
    echo "============================================================"
}

# --- censuslookup:
testutil censuslookup
testutil censuslookup cPcbbbiht
testutil censuslookup fvPQcdecedekrsnrs
testutil censuslookup foo
testutil censuslookup cPcbbbiht foo fvPQcdecedekrsnrs eLPkbdcddhgggb

# --- regconcat with errors:
testutil regconcat
testutil regconcat -o
testutil regconcat -o "$testout"
testutil regconcat -o "$testout" -o
testutil regconcat -o "$testout" -o foo
testutil regconcat -x
testutil regconcat "$testdir"/tiny.rga ''
testutil regconcat -o "$testout" "$testdir"/tiny.rga ''
testutil regconcat -o "$invalidfile" "$testdir"/tiny.rga
# --- regconcat when partly successful:
testutil regconcat "$testdir"/tiny.rga "$invalidfile" "$testdir"/b5.rga
testutil regconcat "$testdir"/tiny.rga "$badfile" "$testdir"/b5.rga
testutil regconcat -o "$testout" "$testdir"/tiny.rga "$invalidfile" "$testdir"/b5.rga
testutil regconcat -o "$testout" "$testdir"/tiny.rga "$badfile" "$testdir"/b5.rga
# --- regconcat when successful:
testutil regconcat "$testdir"/tiny.rga
testutil regconcat "$testdir"/tiny.rga "$testdir"/b5.rga
testutil regconcat -o "$testout" "$testdir"/tiny.rga
testutil regconcat -o "$testout" "$testdir"/tiny.rga "$testdir"/b5.rga

# --- regconvert with errors:
testutil regconvert
testutil regconvert -x -u
testutil regconvert -f
testutil regconvert ''
testutil regconvert -x
testutil regconvert -x "$testdir"/b5.rga
testutil regconvert "$testdir"/b5.rga "$testout" foo
testutil regconvert "$invalidfile"
testutil regconvert "$badfile"
testutil regconvert "$testdir"/b5.rga "$invalidfile"
# --- regconvert when successful:
testutil regconvert "$testdir"/b5.rga
testutil regconvert "$testdir"/tiny.rga
testutil regconvert "$testdir"/b5.rga "$testout"
testutil regconvert "$testdir"/tiny.rga "$testout"
testutil regconvert -x "$testdir"/b5.rga "$testout"
testutil regconvert -x "$testdir"/tiny.rga "$testout"
testutil regconvert -u "$testdir"/b5.rga
testutil regconvert -u "$testdir"/tiny.rga
testutil regconvert -u "$testdir"/b5.rga "$testout"
testutil regconvert -u "$testdir"/tiny.rga "$testout"
# Note: this test framework does not let us test the case old-file == new-file.

# --- regfiledump with errors:
testutil regfiledump
testutil regfiledump -f -l
testutil regfiledump -f -n
testutil regfiledump -l -n
testutil regfiledump -z
testutil regfiledump ''
testutil regfiledump "$invalidfile"
testutil regfiledump "$badfile"
testutil regfiledump -n "$testdir"/tiny.rga foo
# --- regfiledump when successful:
testutil regfiledump "$testdir"/tiny.rga
testutil regfiledump -c "$testdir"/tiny.rga
testutil regfiledump -l "$testdir"/tiny.rga
testutil regfiledump -l -c "$testdir"/tiny.rga
testutil regfiledump -n "$testdir"/tiny.rga
testutil regfiledump "$testdir"/tiny.rga 'Normal Hypersurfaces'
testutil regfiledump "$testdir"/tiny.rga Foo
testutil regfiledump "$testdir"/tiny.rga 'Normal Hypersurfaces' Foo 'Read Me'
testutil regfiledump -l "$testdir"/tiny.rga 'Normal Hypersurfaces'
testutil regfiledump -l "$testdir"/tiny.rga Foo
testutil regfiledump -l "$testdir"/tiny.rga 'Normal Hypersurfaces' Foo 'Read Me'
testutil regfiledump "$testdir"/tags.rga

# --- regfiletype with errors:
testutil regfiletype
# --- regfiletype when successful:
testutil regfiletype "$testdir"/test.rga
testutil regfiletype "$testdir"/8-or.rga
testutil regfiletype "$testdir"/b5.rga
testutil regfiletype "$invalidfile"
testutil regfiletype "$badfile"
testutil regfiletype "$testdir"/test.rga "$testdir"/b5.rga
testutil regfiletype "$testdir"/test.rga "$invalidfile" "$badfile" "$testdir"/b5.rga "$testdir"/8-or.rga

# --- trisetcmp with errors:
testutil trisetcmp
testutil trisetcmp -m -n
testutil trisetcmp -z
testutil trisetcmp "$testdir"/tiny.rga
testutil trisetcmp "$testdir"/tiny.rga ''
testutil trisetcmp '' "$testdir"/tiny.rga
testutil trisetcmp "$testdir"/tiny.rga "$testdir"/test.rga foo
testutil trisetcmp "$testdir"/tiny.rga "$invalidfile"
testutil trisetcmp "$testdir"/tiny.rga "$badfile"
testutil trisetcmp "$invalidfile" "$testdir"/tiny.rga
testutil trisetcmp "$badfile" "$testdir"/tiny.rga
# --- trisetcmp -m when successful:
testutil trisetcmp "$testdir"/tiny.rga "$testdir"/test.rga
testutil trisetcmp "$testdir"/test.rga "$testdir"/tiny.rga
testutil trisetcmp "$testdir"/test.rga "$testdir"/census-or-small.rga
testutil trisetcmp -s "$testdir"/b5.rga "$testdir"/b5-found.rga
testutil trisetcmp -s "$testdir"/b5.rga "$testdir"/8-or.rga
testutil trisetcmp -s "$testdir"/b5-found.rga "$testdir"/b5.rga
testutil trisetcmp -s "$testdir"/test.rga "$testdir"/census-or-small.rga
testutil trisetcmp -s "$testdir"/census-or-small.rga "$testdir"/test.rga
# --- trisetcmp -n when successful:
testutil trisetcmp -n "$testdir"/tiny.rga "$testdir"/test.rga
testutil trisetcmp -n "$testdir"/test.rga "$testdir"/tiny.rga
testutil trisetcmp -n "$testdir"/test.rga "$testdir"/census-or-small.rga
testutil trisetcmp -n -s "$testdir"/test.rga "$testdir"/census-or-small.rga
testutil trisetcmp -n -s "$testdir"/test.rga "$testdir"/b5-found.rga

donetests
