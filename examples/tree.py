# Create a new census of finite orientable closed 3-manifolds with
# two tetrahedra.
from normal.engine.utilities import NBoolSet
census = engine.newNContainer()
engine.formCensus(census, 2, NBoolSet.sTrue, NBoolSet.sTrue, NBoolSet.sFalse)

# Calculate the homology of each triangulation in the census.
# Note that the triangulations are the child packets of the census container.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ": " + str(tri.getHomologyH1())
    tri = tri.getNextTreeSibling()


# Delete each triangulation with trivial homology.
tri = census.getFirstTreeChild()
while tri != None:
    next = tri.getNextTreeSibling()
    hom = tri.getHomologyH1()
    if hom.getRank() == 0 and hom.getNumberOfInvariantFactors() == 0:
        # Remove the triangulation from the tree and destroy it.
        tri.makeOrphan()
        tri.destroy()
    tri = next


# Print the homology of each remaining triangulation.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ": " + str(tri.getHomologyH1())
    tri = tri.getNextTreeSibling()

