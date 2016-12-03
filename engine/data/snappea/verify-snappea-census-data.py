#!/usr/bin/regina-python --nolibs
#
# TODO: Document this.

import sys

if len(sys.argv) != 2:
    print "Usage: " + sys.argv[0] + " <filename>"
    sys.exit(1)

tree = regina.open(sys.argv[1])
if not tree:
    print "E: Could not open file " + sys.argv[1] + "."
    print
    print "Usage: " + sys.argv[0] + " <filename>"
    sys.exit(1)

def process(tri):
    label = tri.label()
    print label

    section = label[0]
    index = int(label[1:])

    manifold = regina.SnapPeaCensusManifold(section, index)
    con = manifold.construct()
    hom = manifold.homology()

    if con == None:
        print 'ERROR: No construction'
        sys.exit(1)
    elif not con.isIsomorphicTo(tri):
        print 'ERROR: Non-isomorphic triangulation'
        sys.exit(1)
    if hom == None:
        print 'ERROR: No homology'
        sys.exit(1)
    elif not hom == tri.homology():
        print 'ERROR: Non-isomorphic homology'
        sys.exit(1)

p = tree
while p != None:
    if p.type() == regina.PACKET_TRIANGULATION3:
        process(p)
    p = p.nextTreePacket()

