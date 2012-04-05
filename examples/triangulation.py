################################
#
#  Sample Python Script
#
#  Illustrates different queries and actions on a 3-manifold triangulation
#  and its normal surfaces.
#
#  See the file "triangulation.session" for the results of running this
#  script.
#
################################

# Create a new (3,4,7) layered solid torus.  This is a 3-tetrahedron
# triangulation of a solid torus.
t = NTriangulation()
t.insertLayeredSolidTorus(3,4)
print t

# Print the full skeleton of the triangulation.
print t.toStringLong()

# Calculate some algebraic properties of the triangulation.
print t.getHomologyH1()
print t.getHomologyH1Bdry()

# Test for 0-efficiency, which asks Regina to search for certain types
# of normal surfaces.
print t.isZeroEfficient()

# Make our own list of vertex normal surfaces in standard coordinates.
surfaces = NNormalSurfaceList.enumerate(t, NNormalSurfaceList.STANDARD)

# Verify that the normal surface list is already a child packet of the
# triangulation.  This happens automatically whenever you enumerate
# normal surfaces (or angle structures).
if surfaces.getTreeParent() == t:
    print "OK: Parent-child relationship is correct."
else:
    print "ERROR: Parent-child relationship is incorrect."


# Print the full list of vertex normal surfaces.
print surfaces.toStringLong()

# Print the Euler characteristic and orientability of each surface.
for i in range(surfaces.getNumberOfSurfaces()):
    s = surfaces.getSurface(i)
    print "Chi =", s.getEulerCharacteristic(), "; Or =", s.isOrientable()


# List all surfaces with more than one quad in the first tetrahedron.
for i in range(surfaces.getNumberOfSurfaces()):
    s = surfaces.getSurface(i)
    if s.getQuadCoord(0,0) + s.getQuadCoord(0,1) + s.getQuadCoord(0,2) > 1:
        print s


# Tidy up.
# Delete the triangulation.  This will automatically delete the surface
# list, which is a child of the triangulation in the packet tree.
t = None
