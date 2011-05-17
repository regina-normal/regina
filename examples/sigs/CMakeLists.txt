
SET(sigsdir ${PKGDATADIR}/examples/sigs)

SET(sigs_files README.txt 1.sig 2.sig 3.sig 4.sig 5.sig 6.sig 7.sig.bz2)
INSTALL(FILES ${sigs_files} DESTINATION ${sigsdir})

