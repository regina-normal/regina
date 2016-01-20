################################
#
#  Sample Python Script
#
#  Illustrates the traversal and manipulation of an entire packet tree.
#
#  See the file "tree.session" for the results of running this script.
#
################################

# Create a new census of all closed orientable 3-manifold
# triangulations with two tetrahedra.
census = NContainer()
NCensus.formCensus(census, 2, NBoolSet.sTrue,
    NBoolSet.sTrue, NBoolSet.sFalse, 0, 0)

# Calculate the homology of each triangulation in the census.
# The triangulations are all children of the "census" container.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ":", tri.homology()
    tri = tri.getNextTreeSibling()


# Remove all triangulations with trivial homology from the tree.
tri = census.getFirstTreeChild()
while tri != None:
    next = tri.getNextTreeSibling()
    hom = tri.homology()
    if hom.isTrivial():
        tri.makeOrphan()
    tri = next


# Print the homology of each remaining triangulation.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ":", tri.homology()
    tri = tri.getNextTreeSibling()

