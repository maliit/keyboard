cmake_minimum_required(VERSION 3.1)

project(maliit-keyboard VERSION 1.9)

option(enable-presage "Use presage to calculate word candidates (maliit-keyboard-plugin only)" ON)
option(enable-hunspell "Use hunspell for error correction (maliit-keyboard-plugin only)" ON)
option(enable-tests "Build tests" OFF)

set(MALIIT_DEFAULT_PROFILE "ubuntu" CACHE STRING "Default keyboard style")

# Install paths
include(GNUInstallDirs)

if(NOT DEFINED QT5_QML_INSTALL_DIR)
    set(QT5_QML_INSTALL_DIR "${LIB_INSTALL_DIR}/qt5/qml" CACHE PATH
            "Installation directory for Qt 5 QML plugins [LIB_INSTALL_DIR/qt5/qml]")
endif()

set(HUNSPELL_DICT_PATH "/usr/share/myspell" CACHE PATH "Directory containing hunspell dictionary files")
set(MALIIT_KEYBOARD_QML_DIR "${CMAKE_INSTALL_LIBDIR}/maliit/keyboard2/qml" CACHE PATH "Directory containing maliit-keyboard QML files")
set(MALIIT_KEYBOARD_DATA_DIR "${CMAKE_INSTALL_DATADIR}/maliit/keyboard2" CACHE PATH "Directory containing maliit-keyboard data files")
set(MALIIT_KEYBOARD_LANGUAGES_DIR "${CMAKE_INSTALL_LIBDIR}/maliit/keyboard2/languages" CACHE PATH "Directory containing maliit-keyboard data")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(MaliitPlugins 0.99.1 REQUIRED)

find_package(GLib2 REQUIRED)
find_package(GIO REQUIRED)

find_package(Qt5DBus)

find_package(Qt5Multimedia)
find_package(Qt5Feedback)

find_package(Anthy)
find_package(Pinyin)
find_package(Chewing)

set(MALIIT_KEYBOARD_LIB_SOURCES
        src/lib/logic/abstractlanguagefeatures.h
        src/lib/logic/abstractlanguageplugin.cpp
        src/lib/logic/abstractlanguageplugin.h
        src/lib/logic/abstractwordengine.cpp
        src/lib/logic/abstractwordengine.h
        src/lib/logic/eventhandler.cpp
        src/lib/logic/eventhandler.h
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

set(WESTERNSUPPORT_SOURCES
        plugins/westernsupport/spellchecker.cpp
        plugins/westernsupport/spellchecker.h
        plugins/westernsupport/westernlanguagefeatures.cpp
        plugins/westernsupport/westernlanguagefeatures.h
        plugins/westernsupport/westernlanguagesplugin.cpp
        plugins/westernsupport/westernlanguagesplugin.h
        plugins/westernsupport/westernsupport.cpp
        plugins/westernsupport/westernsupport.h
        plugins/westernsupport/westernsupport_global.h
        src/lib/logic/abstractlanguageplugin.cpp
        src/lib/logic/abstractlanguageplugin.h)

# TODO install logic/abstractplugininterface.h as HEADERS

set(maliit-keyboard-libraries Qt5::Core)
set(maliit-keyboard-definitions HUNSPELL_DICT_PATH="${HUNSPELL_DICT_PATH}"
        MALIIT_PLUGINS_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_PLUGINS_DATA_DIR}"
        MALIIT_KEYBOARD_QML_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_KEYBOARD_QML_DIR}"
        MALIIT_KEYBOARD_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_KEYBOARD_DATA_DIR}"
        MALIIT_KEYBOARD_LANGUAGES_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_KEYBOARD_LANGUAGES_DIR}")
set(maliit-keyboard-include-dirs)

if(enable-presage)
    find_package(Presage REQUIRED)

    if(PRESAGE_FOUND)
        list(APPEND maliit-keyboard-definitions HAVE_PRESAGE)
        list(APPEND maliit-keyboard-libraries ${PRESAGE_LIBRARIES})
        list(APPEND maliit-keyboard-include-dirs ${PRESAGE_INCLUDE_DIRS})

        list(APPEND WESTERNSUPPORT_SOURCES
                plugins/westernsupport/candidatescallback.cpp
                plugins/westernsupport/candidatescallback.h
                plugins/westernsupport/spellpredictworker.cpp
                plugins/westernsupport/spellpredictworker.h)
    endif()
endif()

if(enable-hunspell)
    find_package(Hunspell REQUIRED)
    list(APPEND maliit-keyboard-definitions HAVE_HUNSPELL)
    list(APPEND maliit-keyboard-libraries ${HUNSPELL_LIBRARIES})
    list(APPEND maliit-keyboard-include-dirs ${HUNSPELL_INCLUDE_DIRS})
endif()

add_library(maliit-keyboard-lib STATIC ${MALIIT_KEYBOARD_LIB_SOURCES})
target_link_libraries(maliit-keyboard-lib Qt5::Core Maliit::Plugins)
target_include_directories(maliit-keyboard-lib PUBLIC src/lib)
target_compile_definitions(maliit-keyboard-lib PRIVATE ${maliit-keyboard-definitions})

set(MALIIT_KEYBOARD_VIEW_SOURCES
        src/view/abstracttexteditor.cpp
        src/view/abstracttexteditor.h
        src/view/setup.cpp
        src/view/setup.h)

add_library(maliit-keyboard-view STATIC ${MALIIT_KEYBOARD_VIEW_SOURCES})
target_link_libraries(maliit-keyboard-view Maliit::Plugins maliit-keyboard-lib)
target_include_directories(maliit-keyboard-view PUBLIC src)

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

set(MALIIT_KEYBOARD_COMMON_SOURCES
        src/plugin/editor.cpp
        src/plugin/editor.h
        src/plugin/feedback.cpp
        src/plugin/feedback.h
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
        src/plugin/theme.cpp src/plugin/theme.h)

add_library(maliit-keyboard-common STATIC ${MALIIT_KEYBOARD_COMMON_SOURCES})
target_link_libraries(maliit-keyboard-common Qt5::DBus Maliit::Plugins maliit-keyboard-lib maliit-keyboard-view gsettings-qt Qt5::Multimedia)
if (Qt5Feedback_FOUND)
    target_link_libraries(maliit-keyboard-common Qt5::Feedback)
    target_compile_definitions(maliit-keyboard-common PUBLIC HAVE_QT5_FEEDBACK)
endif()
target_compile_definitions(maliit-keyboard-common PRIVATE ${maliit-keyboard-definitions})
target_compile_features(maliit-keyboard-common PRIVATE cxx_std_17)

set(MALIIT_KEYBOARD_PLUGIN_SOURCES
        src/plugin/plugin.cpp
        src/plugin/plugin.h
        src/plugin/plugin.json)

add_library(maliit-keyboard-plugin MODULE ${MALIIT_KEYBOARD_PLUGIN_SOURCES})
target_link_libraries(maliit-keyboard-plugin maliit-keyboard-common)
target_compile_definitions(maliit-keyboard-plugin PRIVATE ${maliit-keyboard-definitions})

set(MALIIT_KEYBOARD_SOURCES
        src/plugin/plugin.cpp
        src/plugin/plugin.h
        src/keyboard/keyboard.cpp)

add_executable(maliit-keyboard ${MALIIT_KEYBOARD_SOURCES})
target_link_libraries(maliit-keyboard maliit-keyboard-common)
target_compile_definitions(maliit-keyboard PRIVATE ${maliit-keyboard-definitions})

# TODO install westernlanguagesplugin.h into "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu/include"

add_library(westernsupport STATIC ${WESTERNSUPPORT_SOURCES})
target_link_libraries(westernsupport ${maliit-keyboard-libraries} Maliit::Plugins)
target_include_directories(westernsupport PUBLIC src/lib/logic plugins/westernsupport ${maliit-keyboard-include-dirs})
target_compile_definitions(westernsupport PRIVATE ${maliit-keyboard-definitions})

function(language_plugin _language _full_language _ebook)
    set(PLUGIN_SOURCES
            plugins/${_language}/src/${_full_language}plugin.h
            plugins/${_language}/src/${_full_language}plugin.json)
if(enable-presage)
    add_ngram(TEXT plugins/${_language}/src/${_ebook} DATABASE database_${_language}.db)
    list(APPEND PLUGIN_SOURCES database_${_language}.db)
endif()
    add_library(${_language}plugin MODULE ${PLUGIN_SOURCES})
    target_link_libraries(${_language}plugin westernsupport)
    install(FILES plugins/${_language}/qml/Keyboard_${_language}.qml
            plugins/${_language}/qml/Keyboard_${_language}_email.qml
            plugins/${_language}/qml/Keyboard_${_language}_url.qml
            plugins/${_language}/qml/Keyboard_${_language}_url_search.qml
            DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    install(TARGETS ${_language}plugin
            LIBRARY DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
if(enable-presage)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/database_${_language}.db
            DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
endif()
    foreach(_file IN LISTS ARGN)
        install(FILES plugins/${_language}/${_file}
                DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    endforeach()
endfunction()

function(abstract_language_plugin _language _full_language)
    set(options ABSTRACT_LANGUAGE_PLUGIN)
    set(oneValueArgs NGRAM_DATABASE PLUGIN_DIR LANGUAGE_FEATURES)
    set(multiValueArgs SOURCES LIBRARIES INCLUDE_DIRS DEFINITIONS FILES DIRECTORY)
    cmake_parse_arguments(abstract_language_plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_plugindir ${_language})
    if(NOT ${abstract_language_plugin_PLUGIN_DIR} EQUAL "")
        set(_plugindir ${abstract_language_plugin_PLUGIN_DIR})
    endif()
    set(PLUGIN_SOURCES
            plugins/${_plugindir}/src/${_full_language}plugin.cpp
            plugins/${_plugindir}/src/${_full_language}plugin.h
            plugins/${_plugindir}/src/${_full_language}plugin.json)
    if(NOT ${abstract_language_plugin_LANGUAGE_FEATURES} EQUAL "")
        list(APPEND PLUGIN_SOURCES
                plugins/${_plugindir}/src/${abstract_language_plugin_LANGUAGE_FEATURES}languagefeatures.cpp
                plugins/${_plugindir}/src/${abstract_language_plugin_LANGUAGE_FEATURES}languagefeatures.h)
    else()
        list(APPEND PLUGIN_SOURCES
                plugins/${_plugindir}/src/${_full_language}languagefeatures.cpp
                plugins/${_plugindir}/src/${_full_language}languagefeatures.h)
    endif()
    if(abstract_language_plugin_ABSTRACT_LANGUAGE_PLUGIN)
        list(APPEND PLUGIN_SOURCES
                src/lib/logic/abstractlanguageplugin.cpp
                src/lib/logic/abstractlanguageplugin.h)
    endif()
    foreach(_s IN LISTS abstract_language_plugin_SOURCES)
        list(APPEND PLUGIN_SOURCES plugins/${_plugindir}/src/${_s})
    endforeach()
    if(enable-presage AND (NOT ${abstract_language_plugin_NGRAM_DATABASE} EQUAL ""))
        add_ngram(TEXT plugins/${_plugindir}/src/${abstract_language_plugin_NGRAM_DATABASE} DATABASE database_${_language}.db)
        list(APPEND PLUGIN_SOURCES database_${_language}.db)
    endif()
    add_library(${_language}plugin MODULE ${PLUGIN_SOURCES})
    target_link_libraries(${_language}plugin Qt5::Core Maliit::Plugins ${abstract_language_plugin_LIBRARIES})
    target_include_directories(${_language}plugin PRIVATE src/lib/logic ${abstract_language_plugin_INCLUDE_DIRS})
    target_compile_definitions(${_language}plugin PRIVATE ${abstract_language_plugin_DEFINITIONS})
    install(TARGETS ${_language}plugin
            LIBRARY DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    install(FILES plugins/${_plugindir}/qml/Keyboard_${_language}.qml
            plugins/${_plugindir}/qml/Keyboard_${_language}_email.qml
            plugins/${_plugindir}/qml/Keyboard_${_language}_url.qml
            plugins/${_plugindir}/qml/Keyboard_${_language}_url_search.qml
            DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    foreach(_file IN LISTS abstract_language_plugin_FILES)
        install(FILES plugins/${_plugindir}/${_file}
                DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    endforeach()
    foreach(_dir IN LISTS abstract_language_plugin_DIRECTORY)
        install(DIRECTORY plugins/${_plugindir}/${_dir}
                DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    endforeach()
    if(enable-presage AND (NOT ${abstract_language_plugin_NGRAM_DATABASE} EQUAL ""))
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/database_${_language}.db
                DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    endif()
endfunction()

language_plugin(ar arabic free_ebook.txt)
language_plugin(az azerbaijani free_ebook.txt)
language_plugin(bs bosnian free_ebook.txt)
language_plugin(ca catalan paulina_buxareu.txt src/overrides.csv)
language_plugin(cs czech free_ebook.txt)
language_plugin(da danish free_ebook.txt src/overrides.csv)
language_plugin(de german buddenbrooks.txt)
language_plugin(el greek grazia_deledda-christos_alexandridis.txt)
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
language_plugin(lv latvian free_ebook.txt)
language_plugin(nb norwegian free_ebook.txt)
language_plugin(nl dutch free_ebook.txt src/overrides.csv)
language_plugin(pl polish ziemia_obiecana_tom_pierwszy_4.txt)
language_plugin(pt portuguese historias_sem_data.txt src/overrides.csv)
language_plugin(ro romanian amintiri_din_copilarie.txt)
language_plugin(ru russian free_ebook.txt)
language_plugin(sl slovenian free_ebook.txt)
language_plugin(sr serbian free_ebook.txt)
language_plugin(sv swedish free_ebook.txt src/overrides.csv)
language_plugin(uk ukrainian free_ebook.txt)

abstract_language_plugin(emoji emoji ABSTRACT_LANGUAGE_PLUGIN FILES qml/CategoryKey.qml qml/emoji.js)
if(ANTHY_FOUND)
    abstract_language_plugin(ja japanese ABSTRACT_LANGUAGE_PLUGIN
            SOURCES anthyadapter.cpp anthyadapter.h
            LIBRARIES ${ANTHY_LIBRARIES}
            INCLUDE_DIRS ${ANTHY_INCLUDE_DIRS}
            DIRECTORY qml/keys)
endif()
if(enable-presage)
    abstract_language_plugin(ko korean
            NGRAM_DATABASE korean.txt
            SOURCES database_ko.db
            LIBRARIES westernsupport
            DIRECTORY qml/keys)
endif()
if(PINYIN_FOUND)
    abstract_language_plugin(zh-hans pinyin ABSTRACT_LANGUAGE_PLUGIN
            PLUGIN_DIR pinyin
            LANGUAGE_FEATURES chinese
            SOURCES pinyinadapter.cpp pinyinadapter.h
            LIBRARIES ${PINYIN_LIBRARIES}
            INCLUDE_DIRS ${PINYIN_INCLUDE_DIRS}
            DEFINITIONS PINYIN_DATA_DIR="${PINYIN_DATA_DIR}")
endif()
if(CHEWING_FOUND)
    abstract_language_plugin(zh-hant chewing ABSTRACT_LANGUAGE_PLUGIN
            PLUGIN_DIR chewing
            SOURCES chewingadapter.cpp chewingadapter.h
            LIBRARIES ${CHEWING_LIBRARIES}
            INCLUDE_DIRS ${CHEWING_INCLUDE_DIRS})
endif()

# Installation

install(FILES COPYING COPYING.BSD COPYING.CC-BY README.md
        DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/maliit-keyboard)

install(TARGETS maliit-keyboard-plugin maliit-keyboard
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/maliit/plugins
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY qml/keys qml/languages
        DESTINATION ${MALIIT_KEYBOARD_QML_DIR})
install(FILES qml/constants.js qml/Keyboard.qml qml/KeyboardContainer.qml qml/WordRibbon.qml
        DESTINATION ${MALIIT_KEYBOARD_QML_DIR})

install(DIRECTORY qml/images
        DESTINATION ${MALIIT_KEYBOARD_DATA_DIR})
install(DIRECTORY data/styles
        DESTINATION ${MALIIT_KEYBOARD_DATA_DIR})
install(FILES data/styles/ubuntu/images/icon_backspace@18.png
        DESTINATION ${MALIIT_KEYBOARD_DATA_DIR}/icons
        RENAME erase.png)
install(FILES data/styles/ubuntu/images/icon_enter@18.png
        DESTINATION ${MALIIT_KEYBOARD_DATA_DIR}/icons
        RENAME keyboard-enter.png)
install(FILES data/styles/ubuntu/images/language-icon.png
        DESTINATION ${MALIIT_KEYBOARD_DATA_DIR}/icons
        RENAME language-chooser.png)
install(FILES data/schemas/org.maliit.keyboard.maliit.gschema.xml
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
    target_include_directories(test-utils INTERFACE tests/common tests maliit-keyboard)

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

add_custom_target(install_${PROJECT_NAME}
        ${CMAKE_BUILD_TOOL} install
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME}")
