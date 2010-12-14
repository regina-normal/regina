# manifold 

# Files to compile
set ( FILES ngraphloop ngraphpair ngraphtriple nhandlebody nlensspace nmanifold nsfs nsimplesurfacebundle nsnappeacensusmfd ntorusbundle pymanifold)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
