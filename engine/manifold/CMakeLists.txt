# manifold 

# Files to compile
set ( FILES ngraphloop ngraphpair ngraphtriple nhandlebody nlensspace nmanifold nsfs nsfsaltset nsimplesurfacebundle nsnappeacensusmfd ntorusbundle order)

# Prepend folder name
foreach ( SOURCE_FILE ${FILES} )
  set ( SOURCES ${SOURCES} manifold/${SOURCE_FILE})
endforeach(SOURCE_FILE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
install(FILES ngraphloop.h ngraphpair.h ngraphtriple.h nhandlebody.h nlensspace.h nmanifold.h notation.h nsfs.h nsfsaltset.h nsimplesurfacebundle.h nsnappeacensusmfd.h ntorusbundle.h DESTINATION ${INCLUDEDIR}/manifold)
