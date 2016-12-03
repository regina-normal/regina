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
t = Triangulation3()
t.insertLayeredSolidTorus(3,4)
print t

# Print the full skeleton of the triangulation.
print t.detail()

# Calculate some algebraic properties of the triangulation.
print t.homology()
print t.homologyBdry()

# Test for 0-efficiency, which asks Regina to search for certain types
# of normal surfaces.
print t.isZeroEfficient()

# Make our own list of vertex normal surfaces in standard coordinates.
surfaces = NormalSurfaces.enumerate(t, NS_STANDARD)

# Verify that the normal surface list is already a child packet of the
# triangulation.  This happens automatically whenever you enumerate
# normal surfaces (or angle structures).
if surfaces.parent() == t:
    print "OK: Parent-child relationship is correct."
else:
    print "ERROR: Parent-child relationship is incorrect."


# Print the full list of vertex normal surfaces.
print surfaces.detail()

# Print the Euler characteristic and orientability of each surface.
for i in range(surfaces.size()):
    s = surfaces.surface(i)
    print "Chi =", s.eulerChar(), "; Or =", s.isOrientable()


# List all surfaces with more than one quad in the first tetrahedron.
for i in range(surfaces.size()):
    s = surfaces.surface(i)
    if s.quads(0,0) + s.quads(0,1) + s.quads(0,2) > 1:
        print s


# Tidy up.
# Delete the triangulation.  This will automatically delete the surface
# list, which is a child of the triangulation in the packet tree.
t = None
