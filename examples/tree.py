# Create a new census of finite orientable closed 3-manifolds with
# two tetrahedra.
from regina import NBoolSet
census = regina.NContainer()
regina.NCensus.formCensus(census, 2, NBoolSet.sTrue,
    NBoolSet.sTrue, NBoolSet.sFalse, 0, 0)

# Calculate the homology of each triangulation in the census.
# The triangulations are the child packets of the census container.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ":", tri.getHomologyH1()
    tri = tri.getNextTreeSibling()


# Remove from the tree each triangulation with trivial homology.
tri = census.getFirstTreeChild()
while tri != None:
    next = tri.getNextTreeSibling()
    hom = tri.getHomologyH1()
    if hom.getRank() == 0 and hom.getNumberOfInvariantFactors() == 0:
        tri.makeOrphan()
    tri = next


# Print the homology of each remaining triangulation.
tri = census.getFirstTreeChild()
while tri != None:
    print tri.getPacketLabel() + ":", tri.getHomologyH1()
    tri = tri.getNextTreeSibling()

