# Files to compile
SET ( FILES
  binom.cpp
  cyclotomic.cpp
  laurent.cpp
  laurent2.cpp
  numbertheory.cpp
  perm.cpp
  perm2.cpp
  perm3.cpp
  perm4.cpp
  perm5.cpp
  perm6.cpp
  perm7.cpp
  permconv.cpp
  polynomial.cpp
  )

# Prepend folder name
FOREACH ( SOURCE_FILE ${FILES} )
  SET ( SOURCES ${SOURCES} maths/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE)
SET(SOURCES ${SOURCES} PARENT_SCOPE)

