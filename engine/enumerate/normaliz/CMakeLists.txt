# enumerate/normaliz

# Files to compile
SET ( FILES libnormaliz-impl )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} enumerate/normaliz/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET(SOURCES ${SOURCES} PARENT_SCOPE)

if (${REGINA_INSTALL_DEV})
  INSTALL(FILES cone.cpp cone.h cone_dual_mode.cpp cone_dual_mode.h cone_property.cpp cone_property.h full_cone.cpp full_cone.h integer.cpp integer.h libnormaliz.cpp libnormaliz.h lineare_transformation.cpp lineare_transformation.h list_operations.cpp list_operations.h matrix.cpp matrix.h normaliz_exception.h simplex.cpp simplex.h sublattice_representation.cpp sublattice_representation.h vector_operations.cpp vector_operations.h DESTINATION ${INCLUDEDIR}/enumerate/normaliz)
endif (${REGINA_INSTALL_DEV})
