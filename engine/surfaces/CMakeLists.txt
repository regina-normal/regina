# surfaces

# Files to compile
set ( FILES components crushandcut filters flavours links ndisc ndisctype nnormalsurface nnormalsurfacelist nprism nsanstandard nsquad nsquadoct nsstandard nsurfacefilter nsurfaceset nsurfacesubset nxmlfilterreader nxmlfilterreaders nxmlsurfacereader orientable quadtostd sfcombination sfproperties spheres stdtoquad )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} surfaces/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SURFACES_SOURCES ${SOURCES} PARENT_SCOPE)
