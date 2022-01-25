find_package(Gettext)
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

file(GLOB_RECURSE I18N_SRC_FILES
     RELATIVE ${CMAKE_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/qml/**.qml
     ${CMAKE_SOURCE_DIR}/qml/**.js
     ${CMAKE_SOURCE_DIR}/src/**.cpp
     ${CMAKE_SOURCE_DIR}/src/**.h)
list(SORT I18N_SRC_FILES)

set(DOMAIN ${PROJECT_NAME})
set(POT_FILE ${DOMAIN}.pot)
file(GLOB PO_FILES *.po)

add_custom_target(${POT_FILE}
                  COMMENT "Generating translation template"
                  COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} -o ${POT_FILE}
                          --from-code=UTF-8
                          --c++ --qt --add-comments=TRANSLATORS
                          --keyword=qsTr --keyword=qsTr:1,2 --keyword=N_
                          --package-name='${DOMAIN}'
                          -D ${CMAKE_SOURCE_DIR} ${I18N_SRC_FILES}
                  COMMAND ${CMAKE_COMMAND} -E copy ${POT_FILE} ${CMAKE_CURRENT_SOURCE_DIR})

foreach(PO_FILE ${PO_FILES})
    get_filename_component(LANG ${PO_FILE} NAME_WE)
    gettext_process_po_files(${LANG} ALL PO_FILES ${PO_FILE})
    set(INSTALL_DIR ${CMAKE_INSTALL_LOCALEDIR}/${LANG}/LC_MESSAGES)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.gmo
            DESTINATION ${INSTALL_DIR}
            RENAME ${DOMAIN}.mo)
endforeach(PO_FILE)
