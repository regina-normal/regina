#!/bin/bash
#
# Regina - Software for low-dimensional topology
# Command-Line Utilities Test Suite
#
# Copyright (c) 2016-2025, Ben Burton
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
# If you wish to send your own data to standard input, you can follow the
# arguments with the usual redirection "< filename" (without the quotes).
# You can (and probably should) use @TESTDIR@ in the filename also.
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
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
if [ ! -e "$bindir"/regfiledump -a ! -e "$bindir"/regfiledump.exe ]; then
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
    if [ -x /usr/bin/cygpath.exe ]; then
        # We are running under Windows, which means we need to do some
        # path mangling.  The path to the utility itself will be seen by
        # the utility (via argv[0]) in windows format (C:\...).  All other
        # paths should look like windows paths but with forward slashes
        # (e.g., /foo/foo.rga might become C:/msys64/foo/foo.rga).
        utilpath="`/usr/bin/cygpath -w "$bindir/$util"`" utilname="$util" \
                invalidfile="`/usr/bin/cygpath -m "$invalidfile"`" \
                badfile="`/usr/bin/cygpath -m "$badfile"`" \
                testout="`/usr/bin/cygpath -m "$testout"`" \
                testdir="`/usr/bin/cygpath -m "$testdir"`" \
            perl -pe 's/\Q$ENV{utilpath}\E/$ENV{utilname}/g;
                    s/\Q$ENV{invalidfile}\E/\@INVALIDFILE\@/g;
                    s/\Q$ENV{badfile}\E/\@BADFILE\@/g;
                    s/\Q$ENV{testout}\E/\@TESTOUT\@/g;
                    s/\Q$ENV{testdir}\E/\@TESTDIR\@/g';
    else
        utilpath="$bindir/$util" utilname="$util" \
            perl -pe 's/\Q$ENV{utilpath}\E/$ENV{utilname}/g;
                    s/\Q$ENV{invalidfile}\E/\@INVALIDFILE\@/g;
                    s/\Q$ENV{badfile}\E/\@BADFILE\@/g;
                    s/\Q$ENV{testout}\E/\@TESTOUT\@/g;
                    s/\Q$ENV{testdir}\E/\@TESTDIR\@/g';
    fi
}

# We need the command-line utilities to write unicode output (such as
# packet labels) in UTF-8, so that we can match against our expected output.
if [ "`locale charmap 2>/dev/null`" = UTF-8 ]; then
    # We should be getting UTF-8 by default.
    fix_lang="$LANG"
    fix_lc_all="$LC_ALL"
    fix_lc_ctype="$LC_CTYPE"
else
    # Find ourselves a locale that is UTF-8 and that is supported.
    use_locale=
    backup_locale=
    for i in `locale -a`; do
        upper="`echo "$i" | tr '[:lower:]' '[:upper:]'`"
        case "$upper" in
            UTF-8 ) use_locale="$i"; break ;;
            C.UTF-8 ) use_locale="$i"; break ;;
            C.UTF8 ) use_locale="$i"; break ;;
            *.UTF-8 ) backup_locale="$i" ;;
            *.UTF8 ) backup_locale="$i" ;;
        esac
    done
    if [ -z "$use_locale" ]; then
        use_locale="$backup_locale"
    fi
    if [ -z "$use_locale" ]; then
        echo "ERROR: Could not work out how to enforce UTF-8 output."
        exit 1
    fi
    fix_lang=
    fix_lc_all=
    fix_lc_ctype="$use_locale"
fi

while read -r -a line; do
    util="${line[0]}"
    case "$util" in
        '' ) continue ;;
        '#'* ) continue ;;
    esac

    args=()
    next=
    waiting=0
    infilenext=0
    infile=
    for arg in "${line[@]:1}"; do
        if [ "$infilenext" = 1 ]; then
            if [ -n "$infile" ]; then
              echo "ERROR: Argument after input file: $arg"
              exit 1
            else
              infile="`printf '%s\n' "$arg" | \
                  perl -pe 's/\@TESTDIR\@/$ENV{testdir}/g;'`"
            fi
        elif [ "$waiting" = 0 ]; then
            # We are beginning a new argument.
            case "$arg" in
                "'"*"'" )
                    next="`printf '%s\n' "$arg" | sed -e 's#^.##' -e 's#.$##'`"
                    ;;
                "'"* )
                    next="`printf '%s\n' "$arg" | sed -e 's#^.##'`"
                    waiting=1
                    ;;
                '<' )
                    infilenext=1
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
        if [ "$waiting" = 0 -a "$infilenext" = 0 ]; then
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
    if [ "$infilenext" = 1 -a -z "$infile" ]; then
        echo "ERROR: Missing input file after <"
        exit 1
    fi

    echo "============================================================"
    echo "TEST: ${line[@]}"
    echo "--------------------"
    rm -f "$testout"
    # The LC_ALL setting is to ensure that regina's command-line utilities
    # write unicode strings (e.g., packet labels) in UTF-8, to match how our
    # expected output is encoded.
    if [ -z "$infile" ]; then
      if [ -z "$filter" ]; then
          LANG="$fix_lang" LC_ALL="$fix_lc_all" LC_CTYPE="$fix_lc_ctype" \
              "$bindir/$util" "${args[@]}" 2>&1 | pathfilter && dummy=
          exitcode=$?
      else
          LANG="$fix_lang" LC_ALL="$fix_lc_all" LC_CTYPE="$fix_lc_ctype" \
              "$bindir/$util" "${args[@]}" 2>&1 | pathfilter | \
              "$filter" && dummy=
          exitcode=$?
      fi
    else
      if [ -z "$filter" ]; then
          LANG="$fix_lang" LC_ALL="$fix_lc_all" LC_CTYPE="$fix_lc_ctype" \
              "$bindir/$util" "${args[@]}" < "$infile" 2>&1 | \
              pathfilter && dummy=
          exitcode=$?
      else
          LANG="$fix_lang" LC_ALL="$fix_lc_all" LC_CTYPE="$fix_lc_ctype" \
              "$bindir/$util" "${args[@]}" < "$infile" 2>&1 | \
              pathfilter | "$filter" && dummy=
          exitcode=$?
      fi
    fi
    echo "--------------------"
    echo "Exit code: $exitcode"
    if [ -e "$testout" ]; then
        echo "--------------------"
        magic="`hexdump -n2 -e '/2 "%x\n"' "$testout"`"
        # On s390, the two bytes are output in reverse order.  Try both ways.
        if [ "$magic" = 8b1f -o "$magic" = 1f8b ]; then
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

