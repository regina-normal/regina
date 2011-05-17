
SET(pylibdir ${PKGDATADIR}/pylib)

SET(pylib_files README.txt loadCensus.py packetType.py)
INSTALL(FILES ${pylib_files} DESTINATION ${pylibdir})

