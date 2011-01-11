
SET(pylibdir ${PKGDATADIR}/pylib)

SET(pylib_files README.txt loadCensus.py packetType.py)
EXTRA_DIST(FILES ${pylib_files} DESTINATION ${pylibdir})

