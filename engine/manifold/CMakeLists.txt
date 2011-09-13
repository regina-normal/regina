# manifold 

# Files to compile
SET ( FILES ngraphloop ngraphpair ngraphtriple nhandlebody nlensspace nmanifold nsfs nsfsaltset nsimplesurfacebundle nsnappeacensusmfd ntorusbundle order)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES ngraphloop.h ngraphpair.h ngraphtriple.h nhandlebody.h nlensspace.h nmanifold.h notation.h nsfs.h nsfsaltset.h nsimplesurfacebundle.h nsnappeacensusmfd.h ntorusbundle.h DESTINATION ${INCLUDEDIR}/manifold)
