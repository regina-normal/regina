# utilities

# Files to compile
SET ( FILES base64 hashutils i18nutils nbooleans nthread osutils stringutils xmlutils zstream)

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} utilities/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)

# Set the variable in the parent directory
SET( SOURCES ${SOURCES} PARENT_SCOPE)
INSTALL(FILES base64.h bitmanip.h boostutils.h hashmap.h hashset.h hashutils.h i18nutils.h memutils.h nbitmask.h nbooleans.h nindexedarray.h nlistoncall.h nmarkedvector.h nmatrix2.h nmpi.h nproperty.h nrational.h nthread.h ntrieset.h osutils.h ptrutils.h stlutils.h stringutils.h stringutils.tcc xmlutils.h zstream.h DESTINATION ${INCLUDEDIR}/utilities)
