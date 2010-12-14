# surfaces

# Files to compile
set ( FILES ndisc ndisctype nnormalsurface nnormalsurfacelist nprism  nsurfacefilter nsurfaceset nsurfacesubset sfcombination sfproperties pysurfaces )

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} surfaces/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( SOURCES ${SOURCES} PARENT_SCOPE)
