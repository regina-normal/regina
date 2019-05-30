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
census = Container()
for i in range(415):
    mfd = SnapPeaCensusManifold(SnapPeaCensusManifold.SEC_5, i)
    tri = mfd.construct()
    tri.setLabel(mfd.name())
    census.insertChildLast(tri)

# The triangulations are now all children of the "census" container.
# Remove all triangulations with more than two tetrahedra.
tri = census.firstChild()
while tri != None:
    next = tri.nextSibling()
    if tri.size() > 2:
        tri.makeOrphan()
    tri = next

# Print the homology of each remaining triangulation.
tri = census.firstChild()
while tri != None:
    print tri.label() + ":", tri.homology()
    tri = tri.nextSibling()

