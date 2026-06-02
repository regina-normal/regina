#!/usr/bin/regina-python
#
# Regina - A Normal Surface Theory Calculator
# Database Analysis Utility
#
# Copyright (c) 2026, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
#
# This script reads an uncompressed database from standard input, and outputs
# the largest size of link diagram that the database contains.
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

best = 0
line = sys.stdin.readline()
while line:
    bits = line.split(' ', 1)
    if len(bits) != 2:
        sys.stderr.write('ERROR: Bad input line: ' + line)
        sys.exit(1)

    sig = bits[0]
    link = Link.fromSig(sig)
    if not link:
        sys.stderr.write('ERROR: Invalid signature: ' + sig + '\n')
        sys.exit(1)
    if link.size() > best:
        best = link.size()

    line = sys.stdin.readline()

print(best)
