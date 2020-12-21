
SET(man1dir ${MANDIR}/man1)

SET(man1_files
  censuslookup.1
  regconcat.1
  regconvert.1
  regfiledump.1
  regfiletype.1
  retriangulate.1
  sigcensus.1
  tricensus.1
  trisetcmp.1
  regina-gui.1
  regina-python.1
  )
INSTALL(FILES ${man1_files} DESTINATION ${man1dir} COMPONENT Runtime)

