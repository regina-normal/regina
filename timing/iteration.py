# Regina - A Normal Surface Theory Calculator
# Fine-grained timing for Python iteration
#
# The motivation behind this test is that, in January 2023, SageMath was
# found to impose a significant time penalty on catching a StopIteration
# exception.  This is unfortunate because this is the mechanism by which
# Python iteration is meant to work.  The issue does appear to be specific
# to SageMath, since (using this script) it seems that running the same code
# in a plain Python interpreter does _not_ exhibit the same time penalty.
#
# Copyright (c) 2023, Ben Burton
# For further details contact Ben Burton (bab@debian.org).
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

import regina
import time

t = regina.Example3.weberSeifert()
e = t.edge(0)

print('All times in ms')

# Note: can stop iteration via PyErr_SetString(PyExc_StopIteration, "")

print('--- Start timer: manual iteration ---')
start = time.time()
it = e.embeddings().__iter__()
# print(type(it))
try:
    while True:
        emb = it.__next__()
        print(emb, (time.time() - start) * 1000)
except:
    print('Except:', (time.time() - start) * 1000)
    it = None
print('TOTAL MANUAL:', (time.time() - start) * 1000)

print('--- Start timer: loop iteration ---')
start = time.time()
for emb in e.embeddings():
    print(emb, (time.time() - start) * 1000)
print('TOTAL LOOP:', (time.time() - start) * 1000)

print('--- Start timer: loop iteration over range() ---')
start = time.time()
for i in range(e.degree()):
    print(e.embedding(i), (time.time() - start) * 1000)
print('TOTAL RANGE:', (time.time() - start) * 1000)

class It:
    count = 0
    limit = 0

    def __init__(self, edge):
        self.limit = edge.degree()

    def __iter__(self):
        return self

    def __next__(self):
        if self.count == self.limit:
            raise StopIteration

        ret = self.count
        self.count = self.count + 1
        return ret

print('--- Start timer: pure python iteration ---')
start = time.time()
for i in It(e):
    print(e.embedding(i), (time.time() - start) * 1000)
print('TOTAL PURE PYTHON:', (time.time() - start) * 1000)

