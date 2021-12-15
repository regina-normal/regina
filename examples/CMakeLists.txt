
SET(examplesdir "${PKGDATADIR}/examples")

SET(examples_files
  README.txt
  closed-hyp-census.rga
  closed-or-census.rga
  closed-nor-census.rga
  cusped-hyp-or-census.rga
  cusped-hyp-nor-census.rga
  knot-census.rga
  progress.session
  tree.session
  triangulation.session
  )
INSTALL(FILES ${examples_files} DESTINATION "${examplesdir}")

# Files that come with different versions for python 2 vs python 3:

SET(py23_files
  sample-pyX.rga
  progress-pyX.py
  tree-pyX.py
  triangulation-pyX.py
  )

IF (Python_FOUND)
  IF ("${Python_VERSION_MAJOR}" EQUAL 3)
    SET (PYX py3)
  ELSEIF ("${Python_VERSION_MAJOR}" EQUAL 2)
    SET (PYX py2)
  ELSE ()
    MESSAGE(FATAL_ERROR "Unknown Python version: ${Python_VERSION_MAJOR}")
  ENDIF ()
ELSE (Python_FOUND)
  # Just choose one.
  SET (PYX py3)
ENDIF (Python_FOUND)

FOREACH (FILE ${py23_files})
  STRING (REPLACE "-pyX" "-${PYX}" _SRC "${FILE}")
  STRING (REPLACE "-pyX" "" _DEST "${FILE}")
  INSTALL(FILES ${_SRC} DESTINATION "${examplesdir}" RENAME ${_DEST})
ENDFOREACH (FILE ${py23_files})
