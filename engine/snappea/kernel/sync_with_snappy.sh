#!/bin/bash
#
# Regina - Software for low-dimensional topology
# Upstream synchronisation for the SnapPea kernel
#
# Copyright (c) 2014-2016, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# --------------------------------------------------------------------------
#
# Usage: sync_with_snappy <snappy_source_dir>
#
# Synchronises Regina's SnapPea kernel with the given version of SnapPy,
# by copying the relevant files from SnapPy into the correct locations here.
#
# You should always check the diffs carefully to ensure that you are not
# clobbering any of Regina's own patches to the SnapPea kernel.
#
# --------------------------------------------------------------------------
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
set -e

testfile=cython/SnapPycore.pxi
if ! [ -d "$1" -a -e "$1/$testfile" ]; then
    echo "Usage: $0 <snappy_source_dir>"
    echo
    echo "The directory <snappy_source_dir> should (for instance) contain $testfile."
    exit 1
fi

headerdir="$1/kernel/headers"
sourcedir="$1/kernel/kernel_code"
unixdir="$1/kernel/unix_kit"
snappydir="$1/kernel/addl_code"

unixfiles="unix_file_io.h unix_file_io.c"
snappyfiles="get_gluing_equations.c"
snappymaster="addl_code.h"

for i in "$headerdir"/*.h "$sourcedir"/*.c; do
    src="$i"
    filename=`echo "$src" | sed -e 's#.*/##'`
    if [ -e "$filename" ]; then
      cp "$src" "$filename"
    elif [ -e "unused/$filename" ]; then
      cp "$src" "unused/$filename"
    else
      echo "ERROR: The kernel file "$src" is not mirrored in Regina."
    fi
done

for i in $unixfiles; do
    src="$unixdir/$i"
    filename="$i"
    if [ -e "$filename" ]; then
      cp "$src" "$filename"
    else
      echo "ERROR: The unix_kit file "$src" is not mirrored in Regina."
    fi
done

for i in $snappyfiles; do
    src="$snappydir/$i"
    filename="snappy_$i"
    if [ -e "../snappy/$filename" ]; then
      cp "$src" "../snappy/$filename"
    else
      echo "ERROR: The snappy file "$src" is not mirrored in Regina."
    fi
done

if [ -e "../snappy/SnapPy.h" ]; then
  cp "$snappydir/$snappymaster" "../snappy/SnapPy.h"
else
  echo "ERROR: The snappy header SnapPy.h is missing from Regina."
fi
