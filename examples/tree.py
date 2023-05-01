################################
#
#  Sample Python Script
#
#  Illustrates the traversal and manipulation of an entire packet tree.
#
#  See the file "tree.session" for the results of running this script.
#
################################

# Recreate the original SnapPea census of cusped hyperbolic manifolds
# triangulated by at most 5 tetrahedra.
#
# Since we are building a packet tree, we need to use PacketOfTriangulation3,
# not the plain type Triangulation3 (which is not a packet type).
census = Container()
for i in range(415):
    mfd = SnapPeaCensusManifold(SnapPeaCensusManifold.SEC_5, i)
    census.append(make_packet(mfd.construct(), mfd.name()))

# The triangulations are now all children of the "census" container.
# Remove all triangulations with more than two tetrahedra.
#
# Since we are deleting children, we step through the children manually
# instead of just iterating over children().
tri = census.firstChild()
while tri != None:
    next = tri.nextSibling()
    if tri.size() > 2:
        tri.makeOrphan()
    tri = next

# Print the homology of each remaining triangulation.
# This time we are not adding or removing children, so we can just iterate.
for tri in census.children():
    print(tri.label() + ":", tri.homology())

