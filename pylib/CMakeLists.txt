
set(pylibdir ${PKGDATADIR}/pylib)

set(pylib_files README.txt loadCensus.py packetType.py)
extra_dist(FILES ${pylib_files} DESTINATION ${pylibdir})

