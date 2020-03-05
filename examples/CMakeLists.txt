
SET(examplesdir ${PKGDATADIR}/examples)

SET(examples_files
  README.txt
  closed-hyp-census.rga
  closed-or-census.rga
  closed-nor-census.rga
  cusped-hyp-or-census.rga
  cusped-hyp-nor-census.rga
  hyp-knot-link-census.rga
  )
INSTALL(FILES ${examples_files} DESTINATION ${examplesdir})

# Files that come with different versions for python 2 vs python 3:

SET(py23_files
  sample-pyX.rga
  progress-pyX.py
  progress-pyX.session
  tree-pyX.py
  tree-pyX.session
  triangulation-pyX.py
  triangulation-pyX.session
  )

IF (PYTHONINTERP_FOUND AND "${PYTHON_VERSION_MAJOR}" EQUAL 3)
  SET (PYX py3)
ELSE ()
  SET (PYX py2)
ENDIF ()

FOREACH (FILE ${py23_files})
  STRING (REPLACE "-pyX" "-${PYX}" _SRC "${FILE}")
  STRING (REPLACE "-pyX" "" _DEST "${FILE}")
  INSTALL(FILES ${_SRC} DESTINATION ${examplesdir} RENAME ${_DEST})
ENDFOREACH (FILE ${py23_files})
