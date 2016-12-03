set (IMG_FILES )

set (IMGS
  application-exit
  application-xml
  arrow-down
  arrow-down-double
  arrow-left
  arrow-right
  arrow-up
  arrow-up-double
  bookmarks
  clock
  configure
  dialog-cancel
  dialog-information
  dialog-ok
  dialog-warning
  document-new
  document-open
  document-open-recent
  document-save
  document-save-as
  edit-copy
  edit-cut
  edit-delete
  edit-paste
  edit-rename
  edit-select-all
  edit-table-delete-row
  edit-table-insert-row-below
  go-bottom
  go-top
  help-about
  help-contents
  help-contextual
  help-hint
  mail-attachment
  process-stop
  system-run
  text-x-c++src
  utilities-terminal
  window-close
  )
set (SIZES 16 16@2x 22 22@2x 32 32@2x 48 48@2x 64 64@2x 128 128@2x)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
      SET (IMG_FILES ${IMG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} DESTINATION ${PKGDATADIR}/icons/oxygen)

