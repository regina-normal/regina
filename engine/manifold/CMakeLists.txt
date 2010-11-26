# manifold 

# Files to compile
set ( FILES ngraphloop ngraphpair ngraphtriple nhandlebody nlensspace nmanifold nsfs nsfsaltset nsimplesurfacebundle nsnappeacensusmfd ntorusbundle order)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
endforeach(SOURCE_FILE)

# Set the variable in the parent directory
set( MANIFOLD_SOURCES ${SOURCES} PARENT_SCOPE)
