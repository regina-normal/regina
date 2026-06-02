#!/usr/bin/regina-python
#
# Regina - A Normal Surface Theory Calculator
# Database Conversion Utility
#
# Copyright (c) 2026, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# This script reads an uncompressed database from standard input, converts
# all signatures to second-generation signatures, and writes the uncompressed
# database to standard output.
#
# The script was written to support the migration of census databases from
# first-generation to second-generation signatures in 2026 (for Regina 8.0).
# It has been archived here in case it ever becomes useful again later.
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

import sys

line = sys.stdin.readline()
while line:
    bits = line.split(' ', 1)
    if len(bits) != 2:
        sys.stderr.write('ERROR: Bad input line: ' + line)
        sys.exit(1)

    sig = bits[0]
    name = bits[1]

    tri = Triangulation3.fromSig(sig)
    if not tri:
        sys.stderr.write('ERROR: Invalid signature: ' + sig + '\n')
        sys.exit(1)

    sys.stdout.write(tri.neoSig() + ' ' + name)

    line = sys.stdin.readline()

