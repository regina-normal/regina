# Create a new (3,4,7) layered solid torus.
t = engine.newNTriangulation()
t.insertLayeredSolidTorus(3,4)
t

# Dump skeletal information for the triangulation.
print t.toStringLong()

# Calculate various properties of the triangulation.
t.getHomologyH1()
t.getHomologyH1Bdry()
t.isZeroEfficient()

# Get a list of normal surfaces in standard tri-quad coordinates.
from normal.engine.surfaces import NNormalSurfaceList
surfaces = engine.newNNormalSurfaceList(t, NNormalSurfaceList.STANDARD)

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
t.destroy()

