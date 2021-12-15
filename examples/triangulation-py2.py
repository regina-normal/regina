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
t = Example3.lst(3, 4)
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
surfaces = NormalSurfaces(t, NS_STANDARD)

# Print the full list of vertex normal surfaces.
print surfaces.detail()

# Print the Euler characteristic and orientability of each surface.
for s in surfaces:
    print "Chi =", s.eulerChar(), "; Or =", s.isOrientable()


# List all surfaces with more than one quad in the first tetrahedron.
for s in surfaces:
    if s.quads(0,0) + s.quads(0,1) + s.quads(0,2) > 1:
        print s

