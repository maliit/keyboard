cmake_minimum_required(VERSION 3.1)

project(ubuntu-keyboard VERSION 0.1)

option(enable-presage "Use presage to calculate word candidates (maliit-keyboard-plugin only)" ON)
option(enable-hunspell "Use hunspell for error correction (maliit-keyboard-plugin only)" ON)
option(enable-tests "Build tests" OFF)

set(MALIIT_DEFAULT_PROFILE "ubuntu" CACHE STRING "Default keyboard style")

if(NOT DEFINED INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include" CACHE PATH
            "Installation directory for C/C++ header files [CMAKE_INSTALL_PREFIX/include]")
endif()

if(NOT DEFINED LIB_INSTALL_DIR)
    set(LIB_INSTALL_DIR "lib" CACHE PATH
            "Installation directory for object code libraries [CMAKE_INSTALL_PREFIX/lib]")
endif()

if(NOT DEFINED SHARE_INSTALL_PREFIX)
    set(SHARE_INSTALL_PREFIX "share" CACHE PATH
            "Installation directory for read-only architecture-independent data [CMAKE_INSTALL_PREFIX/share]")
endif()

if(NOT DEFINED QT5_QML_INSTALL_DIR)
    set(QT5_QML_INSTALL_DIR "${LIB_INSTALL_DIR}/qt5/qml" CACHE PATH
            "Installation directory for Qt 5 QML plugins [LIB_INSTALL_DIR/qt5/qml]")
endif()

set(HUNSPELL_DICT_PATH "/usr/share/myspell" CACHE PATH "Directory containing hunspell dictionary files")
set(MALIIT_PLUGINS_DATA_DIR "${SHARE_INSTALL_PREFIX}/maliit/plugins" CACHE PATH "Directory containing maliit-plugins data")
set(UBUNTU_KEYBOARD_DATA_DIR "${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu" CACHE PATH "Directory containing ubuntu-keyboard data")
set(UBUNTU_KEYBOARD_LIB_DIR "${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu/lib" CACHE PATH "Directory containing ubuntu-keyboard data")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(MaliitPlugins 0.99.1 REQUIRED)

find_package(GLib2 REQUIRED)
find_package(GIO REQUIRED)

find_package(Qt5DBus)

set(UBUNTU_KEYBOARD_SOURCES
        src/lib/logic/abstractlanguagefeatures.h
        src/lib/logic/abstractlanguageplugin.cpp
        src/lib/logic/abstractlanguageplugin.h
        src/lib/logic/abstractwordengine.cpp
        src/lib/logic/abstractwordengine.h
        src/lib/logic/eventhandler.cpp
        src/lib/logic/eventhandler.h
        src/lib/logic/languageplugininterface.h
        src/lib/logic/style.cpp
        src/lib/logic/style.h
        src/lib/logic/wordengine.cpp
        src/lib/logic/wordengine.h

        src/lib/models/area.cpp
        src/lib/models/area.h
        src/lib/models/key.cpp
        src/lib/models/key.h
        src/lib/models/keyarea.cpp
        src/lib/models/keyarea.h
        src/lib/models/keyboard.h
        src/lib/models/keydescription.h
        src/lib/models/layout.cpp
        src/lib/models/layout.h
        src/lib/models/styleattributes.cpp
        src/lib/models/styleattributes.h
        src/lib/models/text.cpp
        src/lib/models/text.h
        src/lib/models/wordcandidate.cpp
        src/lib/models/wordcandidate.h
        src/lib/models/wordribbon.cpp
        src/lib/models/wordribbon.h

        src/lib/coreutils.cpp
        src/lib/coreutils.h)

# TODO install logic/languageplugininterface.h and logic/abstractplugininterface.h as HEADERS

set(ubuntu-keyboard-libraries Qt5::Core)
set(ubuntu-keyboard-definitions HUNSPELL_DICT_PATH="${HUNSPELL_DICT_PATH}"
        MALIIT_PLUGINS_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_PLUGINS_DATA_DIR}"
        UBUNTU_KEYBOARD_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${UBUNTU_KEYBOARD_DATA_DIR}"
        UBUNTU_KEYBOARD_LIB_DIR="${CMAKE_INSTALL_PREFIX}/${UBUNTU_KEYBOARD_LIB_DIR}")
set(ubuntu-keyboard-include-dirs)

if(enable-presage)
    find_package(Presage REQUIRED)
    if(PRESAGE_FOUND)
        list(APPEND ubuntu-keyboard-definitions HAVE_PRESAGE)
        list(APPEND ubuntu-keyboard-libraries ${PRESAGE_LIBRARIES})
        list(APPEND ubuntu-keyboard-include-dirs ${PRESAGE_INCLUDE_DIRS})
    endif()
endif()

if(enable-hunspell)
    find_package(Hunspell REQUIRED)
    list(APPEND ubuntu-keyboard-definitions HAVE_HUNSPELL)
    list(APPEND ubuntu-keyboard-libraries ${HUNSPELL_LIBRARIES})
    list(APPEND ubuntu-keyboard-include-dirs ${HUNSPELL_INCLUDE_DIRS})
endif()

add_library(ubuntu-keyboard STATIC ${UBUNTU_KEYBOARD_SOURCES})
target_link_libraries(ubuntu-keyboard ${ubuntu-keyboard-libraries} Maliit::Plugins)
target_include_directories(ubuntu-keyboard PUBLIC src/lib)
target_include_directories(ubuntu-keyboard PRIVATE ${ubuntu-keyboard-include-dirs})
target_compile_definitions(ubuntu-keyboard PRIVATE ${ubuntu-keyboard-definitions})

set(UBUNTU_KEYBOARD_VIEW_SOURCES
        src/view/abstracttexteditor.cpp
        src/view/abstracttexteditor.h
        src/view/setup.cpp
        src/view/setup.h)

add_library(ubuntu-keyboard-view STATIC ${UBUNTU_KEYBOARD_VIEW_SOURCES})
target_link_libraries(ubuntu-keyboard-view Maliit::Plugins ubuntu-keyboard)
target_include_directories(ubuntu-keyboard-view PUBLIC src)

set(QGSETTINGS_SOURCES
        src/3rdparty/QGSettings/qconftypes.cpp
        src/3rdparty/QGSettings/qconftypes.h
        src/3rdparty/QGSettings/QGSettings
        src/3rdparty/QGSettings/qgsettings.cpp
        src/3rdparty/QGSettings/qgsettings.h
        src/3rdparty/QGSettings/util.cpp
        src/3rdparty/QGSettings/util.h)

add_library(gsettings-qt STATIC ${QGSETTINGS_SOURCES})
target_link_libraries(gsettings-qt Qt5::Core ${GIO_LIBRARIES})
target_include_directories(gsettings-qt PUBLIC ${GIO_INCLUDE_DIRS} src/3rdparty)
target_compile_definitions(gsettings-qt PRIVATE -DQT_NO_KEYWORDS)

set(UBUNTU_KEYBOARD_PLUGIN_SOURCES
        src/plugin/editor.cpp
        src/plugin/editor.h
        src/plugin/greeterstatus.cpp
        src/plugin/greeterstatus.h
        src/plugin/updatenotifier.cpp
        src/plugin/updatenotifier.h
        src/plugin/inputmethod.cpp
        src/plugin/inputmethod.h
        src/plugin/inputmethod_p.h
        src/plugin/keyboardgeometry.cpp
        src/plugin/keyboardgeometry.h
        src/plugin/keyboardsettings.cpp
        src/plugin/keyboardsettings.h
        src/plugin/plugin.cpp
        src/plugin/plugin.h
        src/plugin/units.cpp
        src/plugin/units.h
        src/plugin/plugin.json)

add_library(ubuntu-keyboard-plugin MODULE ${UBUNTU_KEYBOARD_PLUGIN_SOURCES})
target_link_libraries(ubuntu-keyboard-plugin Qt5::DBus Maliit::Plugins ubuntu-keyboard ubuntu-keyboard-view gsettings-qt)
target_compile_definitions(ubuntu-keyboard-plugin PRIVATE ${ubuntu-keyboard-definitions})

set(WESTERNSUPPORT_SOURCES
        plugins/westernsupport/candidatescallback.cpp
        plugins/westernsupport/candidatescallback.h
        plugins/westernsupport/spellchecker.cpp
        plugins/westernsupport/spellchecker.h
        plugins/westernsupport/spellpredictworker.cpp
        plugins/westernsupport/spellpredictworker.h
        plugins/westernsupport/westernlanguagefeatures.cpp
        plugins/westernsupport/westernlanguagefeatures.h
        plugins/westernsupport/westernlanguagesplugin.cpp
        plugins/westernsupport/westernlanguagesplugin.h
        plugins/westernsupport/westernsupport.cpp
        plugins/westernsupport/westernsupport.h
        plugins/westernsupport/westernsupport_global.h
        src/lib/logic/abstractlanguageplugin.cpp
        src/lib/logic/abstractlanguageplugin.h)

# TODO install westernlanguagesplugin.h into "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu/include"

add_library(westernsupport STATIC ${WESTERNSUPPORT_SOURCES})
target_link_libraries(westernsupport Qt5::Core Maliit::Plugins ${PRESAGE_LIBRARIES})
target_include_directories(westernsupport PUBLIC src/lib/logic plugins/westernsupport)
target_compile_definitions(westernsupport PRIVATE -DHUNSPELL_DICT_PATH="${HUNSPELL_DICT_PATH}")

function(language_plugin _language _full_language _ebook)
    add_ngram(TEXT plugins/${_language}/src/${_ebook} DATABASE database_${_language}.db)
    add_library(${_language}plugin MODULE plugins/${_language}/src/${_full_language}plugin.h
            database_${_language}.db
            plugins/${_language}/src/${_full_language}plugin.json)
    target_link_libraries(${_language}plugin westernsupport)
    install(FILES plugins/${_language}/qml/Keyboard_${_language}.qml
            plugins/${_language}/qml/Keyboard_${_language}_email.qml
            plugins/${_language}/qml/Keyboard_${_language}_url.qml
            plugins/${_language}/qml/Keyboard_${_language}_url_search.qml
            DESTINATION ${UBUNTU_KEYBOARD_LIB_DIR}/${_language})
    install(TARGETS ${_language}plugin
            LIBRARY DESTINATION ${UBUNTU_KEYBOARD_LIB_DIR}/${_language})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/database_${_language}.db
            DESTINATION ${UBUNTU_KEYBOARD_LIB_DIR}/${_language})
    foreach(_file IN LISTS ARGN)
        install(FILES plugins/${_language}/${_file}
                DESTINATION ${UBUNTU_KEYBOARD_LIB_DIR}/${_language})
    endforeach()
endfunction()

language_plugin(ar arabic free_ebook.txt)
language_plugin(az azerbaijani free_ebook.txt)
language_plugin(bs bosnian free_ebook.txt)
language_plugin(ca catalan paulina_buxareu.txt src/overrides.csv)
#language_plugin(chewing)
# TODO build chewing plugin
language_plugin(cs czech free_ebook.txt)
language_plugin(da danish free_ebook.txt src/overrides.csv)
language_plugin(de german buddenbrooks.txt)
language_plugin(el greek grazia_deledda-christos_alexandridis.txt)
#language_plugin(emoji)
# TODO build emoji plugin
language_plugin(en english the_picture_of_dorian_gray.txt src/overrides.csv)
language_plugin(eo esperanto alicio_en_mirlando.txt)
language_plugin(es spanish el_quijote.txt)
language_plugin(fa persian free_ebook.txt qml/Keyboard_symbols_fa.qml)
language_plugin(fi finnish free_ebook.txt)
language_plugin(fr french les_trois_mousquetaires.txt src/overrides.csv)
language_plugin(gd gaelic teacsa.txt)
language_plugin(he hebrew free_ebook.txt src/overrides.csv)
language_plugin(hr croatian knjiga.txt)
language_plugin(hu hungarian free_ebook.txt)
language_plugin(is icelandic althingi_umraedur_2004_2005.txt)
language_plugin(it italian la_francia_dal_primo_impero.txt src/overrides.csv)
#language_plugin(ja)
#language_plugin(ko)
# TODO build ja and ko plugins
language_plugin(lv latvian free_ebook.txt)
language_plugin(nb norwegian free_ebook.txt)
language_plugin(nl dutch free_ebook.txt src/overrides.csv)
#language_plugin(pinyin)
# TODO build pinyin plugin
language_plugin(pl polish ziemia_obiecana_tom_pierwszy_4.txt)
language_plugin(pt portuguese historias_sem_data.txt src/overrides.csv)
language_plugin(ro romanian amintiri_din_copilarie.txt)
language_plugin(ru russian free_ebook.txt)
language_plugin(sl slovenian free_ebook.txt)
language_plugin(sr serbian free_ebook.txt)
language_plugin(sv swedish free_ebook.txt src/overrides.csv)
language_plugin(uk ukrainian free_ebook.txt)

# Installation

install(FILES COPYING COPYING.BSD COPYING.CC-BY README
        DESTINATION ${SHARE_INSTALL_PREFIX}/doc/ubuntu-keyboard)

install(TARGETS ubuntu-keyboard-plugin
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}/maliit/plugins)

install(DIRECTORY qml/images qml/keys qml/languages
        DESTINATION ${UBUNTU_KEYBOARD_DATA_DIR})
install(FILES qml/constants.js qml/Keyboard.qml qml/KeyboardContainer.qml qml/WordRibbon.qml
        DESTINATION ${UBUNTU_KEYBOARD_DATA_DIR})

install(DIRECTORY data/styles
        DESTINATION ${UBUNTU_KEYBOARD_DATA_DIR})
install(FILES data/schemas/com.canonical.keyboard.maliit.gschema.xml
        DESTINATION share/glib-2.0/schemas/)

# TODO add all tests
if(enable-tests)
    enable_testing()

    find_package(Qt5Test)

    add_library(test-utils STATIC
            tests/unittests/common/inputmethodhostprobe.cpp
            tests/unittests/common/inputmethodhostprobe.h
            tests/unittests/common/utils-gui.cpp
            tests/unittests/common/utils.cpp
            tests/unittests/common/utils.h)

    target_link_libraries(test-utils PUBLIC Qt5::Core Qt5::Gui Qt5::Test)
    target_include_directories(test-utils INTERFACE tests/common tests ubuntu-keyboard)

    function(create_test name)
        set(_extra_sources ${ARGV})
        list(REMOVE_AT _extra_sources 0)

        add_executable(${name}
                tests/unittests/${name}/${name}.cpp
                ${_extra_sources})

        target_link_libraries(${name} test-utils ${_libs})

        add_test(${name} ${name})

        set(test_targets ${test_targets} ${name} PARENT_SCOPE)
    endfunction()

    create_test(ut_languagefeatures)
    create_test(ut_repeat-backspace)
    create_test(ut_editor
            tests/unittests/ut_editor/wordengineprobe.cpp
            tests/unittests/ut_editor/wordengineprobe.h)
    create_test(ut_preedit-string)
    create_test(ut_text)
    create_test(ut_word-candidates
            tests/unittests/ut_word-candidates/wordengineprobe.cpp
            tests/unittests/ut_word-candidates/wordengineprobe.h)
    create_test(ut_wordengine)

    set_property(TEST ${test_targets} PROPERTY ENVIRONMENT
            MALIIT_PLUGINS_DATADIR=${CMAKE_SOURCE_DIR}/data)

endif()
