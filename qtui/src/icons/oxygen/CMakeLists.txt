set (IMG_FILES )

set (IMGS application-exit application-xml arrow-down arrow-down-double arrow-left arrow-right arrow-up arrow-up-double bookmarks clock configure dialog-cancel dialog-information dialog-ok dialog-warning document-new document-open document-open-recent document-save document-save-as document-sign edit-copy edit-cut edit-delete edit-find edit-paste edit-rename edit-select-all edit-table-delete-row edit-table-insert-row-below emblem-locked go-bottom go-top help-about help-contents help-contextual help-hint list-add list-remove mail-attachment process-stop run-build-file system-run text-x-c++src tools-wizard utilities-terminal view-list-text view-refresh window-close)
set (SIZES 8 16 22 32 48 64 128)

foreach (IMG_CLASS ${IMGS})
  foreach (IMG_SIZE ${SIZES})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
      SET (IMG_FILES ${IMG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${IMG_CLASS}-${IMG_SIZE}.png)
  endforeach (IMG_SIZE ${SIZES})
endforeach (IMG_CLASS ${IMGS})

INSTALL (FILES ${IMG_FILES} DESTINATION ${PKGDATADIR}/icons/oxygen)

