################################
#
#  Sample Python Script
#
#  Illustrates the traversal and manipulation of an entire packet tree.
#
#  See the file "tree.session" for the results of running this script.
#
################################

# Create a new census of finite orientable closed 3-manifold
# triangulations with two tetrahedra.
from regina import NBoolSet
census = regina.NContainer()
regina.NCensus.formCensus(census, 2, NBoolSet.sTrue,
    NBoolSet.sTrue, NBoolSet.sFalse, 0, 0)

# Calculate the homology of each triangulation in the census.
# Note that formCensus() will have inserted each new triangulation as
# a child packet of the overall census container.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ":", tri.getHomologyH1()
    tri = tri.getNextTreeSibling()


# Remove from the tree each triangulation with trivial homology.
tri = census.getFirstTreeChild()
while tri != None:
    next = tri.getNextTreeSibling()
    hom = tri.getHomologyH1()
    if hom.isTrivial():
        tri.makeOrphan()
    tri = next


# Print the homology of each remaining triangulation.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ":", tri.getHomologyH1()
    tri = tri.getNextTreeSibling()

