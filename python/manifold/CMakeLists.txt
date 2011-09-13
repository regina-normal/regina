# manifold 

# Files to compile
SET ( FILES ngraphloop ngraphpair ngraphtriple nhandlebody nlensspace nmanifold nsfs nsimplesurfacebundle nsnappeacensusmfd ntorusbundle pymanifold)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)
