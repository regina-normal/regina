################################
#
#  Sample Python Script
#
#  Illustrates various queries and actions that can be performed upon a
#  3-manifold triangulation and its normal surfaces.
#
#  See the file "triangulation.session" for the results of running this
#  script.
#
################################

# Create a new (3,4,7) layered solid torus.  This is a 3-tetrahedron
# triangulation of a solid torus.
t = regina.NTriangulation()
t.insertLayeredSolidTorus(3,4)
print t

# Dump skeletal information for the triangulation.
print t.toStringLong()

# Calculate some algebraic properties of the triangulation.
print t.getHomologyH1()
print t.getHomologyH1Bdry()

# Test for 0-efficiency, which involves searching for particular types
# of normal surface.
print t.isZeroEfficient()

# Get a list of normal surfaces in standard tri-quad coordinates.
from regina import NNormalSurfaceList
surfaces = NNormalSurfaceList.enumerate(t, NNormalSurfaceList.STANDARD)

# Verify that the normal surface list has been made a child packet of the
# triangulation.  This happens automatically whenever the normal
# surfaces (or angle structures) of a triangulation are enumerated.
if surfaces.getTreeParent() == t:
    print "OK: Parent-child relationship is correct."
else:
    print "ERROR: Parent-child relationship is incorrect."


# Dump the entire list of vertex normal surfaces.
print surfaces.toStringLong()

# Print the Euler characteristic and orientability of each surface.
for i in range(surfaces.getNumberOfSurfaces()):
    s = surfaces.getSurface(i)
    print "Chi =", s.getEulerCharacteristic(), "; Or =", s.isOrientable()


# List all the surfaces with more than one quad in the first tetrahedron.
for i in range(surfaces.getNumberOfSurfaces()):
    s = surfaces.getSurface(i)
    if s.getQuadCoord(0,0) + s.getQuadCoord(0,1) + s.getQuadCoord(0,2) > 1:
        print s


# Tidy up.
# Delete the triangulation; this will automatically delete the surface
# list since the surface list is a child packet of the triangulation.
t = None

