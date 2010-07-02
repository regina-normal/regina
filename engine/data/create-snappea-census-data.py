#!/usr/bin/regina-python --nolibs
#
# Reads the given file and outputs a line for each triangulation.
# Each line contains two alphabetical strings, encoding the face gluings
# and the first homology group respectively.
#
# The face gluings are encoded using the Callahan-Hildebrand-Weeks
# dehydration format.  The first homology group is encoded by a string
# of letters, where a..z represent 0..25 and A..Z represent 26..51.
# The first letter gives the rank of the group, and the remaining
# letters give the invariant factors.

import sys

if len(sys.argv) != 2:
    print "Usage: " + sys.argv[0] + " <filename>"
    sys.exit(1)

tree = regina.readFileMagic(sys.argv[1])
if not tree:
    print "E: Could not open file " + sys.argv[1] + "."
    print
    print "Usage: " + sys.argv[0] + " <filename>"
    sys.exit(1)

def encode(i):
    if i < 0:
        print 'ERROR: Negative integer:', i
        sys.exit(1)
    if i < 26:
        return chr(ord('a') + i)
    if i < 52:
        return chr(ord('A') + i - 26)
    print 'ERROR: Integer out of range:', i
    sys.exit(1)

def grouprep(g):
    ans = encode(g.getRank())
    for i in range(g.getNumberOfInvariantFactors()):
        ans = ans + encode(g.getInvariantFactor(i).longValue())
    return ans

def process(tri):
    dehydration = tri.dehydrate()
    if dehydration == '':
        print 'ERROR: No dehydration'
        sys.exit(1)
    print dehydration + ' ' + grouprep(tri.getHomologyH1())
    sys.stdout.flush()

p = tree
while p != None:
    if p.getPacketType() == regina.NTriangulation.packetType:
        process(p)
    p = p.nextTreePacket()

