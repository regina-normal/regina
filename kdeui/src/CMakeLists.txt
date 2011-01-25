# 

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} ${KDE4_INCLUDE_DIRS} ${ENGINE_INCLUDES} ${PROJECT_BINARY_DIR}/engine)
ADD_DEFINITIONS( ${KDE4_DEFINITIONS} )
INCLUDE(${QT_USE_FILE})

# Common files, creates a variable holding the list of common source files
ADD_SUBDIRECTORY(common)

ADD_SUBDIRECTORY(part)
#ADD_SUBDIRECTORY(shell)




# Messages.sh.in based on script from 
# http://techbase.kde.org/Development/Tutorials/Localization/i18n_Build_Systems

CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/kdeui/src/Messages.sh.in"
  "${PROJECT_BINARY_DIR}/kdeui/src/Messages.sh"
  @ONLY
  )

EXECUTE_PROCESS(
  COMMAND "${PROJECT_BINARY_DIR}/kdeui/src/Messages.sh"
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/kdeui/src"
)

# Following taken from http://techbase.kde.org/Development/Tutorials/Localization/i18n_Build_Systems

FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
 
IF(NOT GETTEXT_MSGFMT_EXECUTABLE)
        MESSAGE(
"------
                 NOTE: msgfmt not found. Translations will *not* be installed
------")
ELSE(NOT GETTEXT_MSGFMT_EXECUTABLE)
 
        SET(catalogname regina)
 
        ADD_CUSTOM_TARGET(translations ALL)
        
 
        FILE(GLOB PO_FILES *.po)
 
        FOREACH(_poFile ${PO_FILES})
                GET_FILENAME_COMPONENT(_poFileName ${_poFile} NAME)
                STRING(REGEX REPLACE "^${catalogname}_?" "" _langCode ${_poFileName} )
                STRING(REGEX REPLACE "\\.po$" "" _langCode ${_langCode} )
 
                IF( _langCode )
                        GET_FILENAME_COMPONENT(_lang ${_poFile} NAME_WE)
                        SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)
 
                        ADD_CUSTOM_COMMAND(TARGET translations
                                COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_poFile}
                                DEPENDS ${_poFile})
                        INSTALL(FILES ${_gmoFile} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${catalogname}.mo)
                ENDIF( _langCode )
 
        ENDFOREACH(_poFile ${PO_FILES})
 
ENDIF(NOT GETTEXT_MSGFMT_EXECUTABLE)
