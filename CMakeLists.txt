cmake_minimum_required(VERSION 3.9)

project(maliit-keyboard VERSION 2.3.1)

option(BUILD_WITH_QT6 "Enable build with Qt6" OFF)
set(MALIIT_SUFFIX "")
if(BUILD_WITH_QT6)
  find_package(Qt6 6.0 REQUIRED COMPONENTS Core Core5Compat QuickControls2)
  find_package(Qt6DBus)
  find_package(Qt6Multimedia)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(Qt6_FOUND)
  set(QT_VERSION_MAJOR 6)
  set(MALIIT_SUFFIX "6")
  message(STATUS "Building with Qt6")
else()
  find_package(Qt5 5.14 REQUIRED COMPONENTS Core QuickControls2)
  find_package(Qt5DBus)
  find_package(Qt5Multimedia)
  find_package(Qt5Feedback)
  set(QT_VERSION_MAJOR 5)
  message(STATUS "Building with Qt5")
endif()

option(enable-hunspell "Use hunspell for error correction (maliit-keyboard-plugin only)" ON)
option(enable-tests "Build tests" ON)

# Install paths
include(GNUInstallDirs)

set(HUNSPELL_DICT_PATH "/usr/share/hunspell" CACHE PATH "Directory containing hunspell dictionary files")
set(MALIIT_KEYBOARD_QML_DIR "${CMAKE_INSTALL_LIBDIR}/maliit${MALIIT_SUFFIX}/keyboard2/qml" CACHE PATH "Directory containing maliit-keyboard QML files")
set(MALIIT_KEYBOARD_DATA_DIR "${CMAKE_INSTALL_DATADIR}/maliit${MALIIT_SUFFIX}/keyboard2" CACHE PATH "Directory containing maliit-keyboard data files")
set(MALIIT_KEYBOARD_LANGUAGES_DIR "${CMAKE_INSTALL_LIBDIR}/maliit${MALIIT_SUFFIX}/keyboard2/languages" CACHE PATH "Directory containing maliit-keyboard data")

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Maliit${MALIIT_SUFFIX}Plugins 0.99.1 REQUIRED)

find_package(GLib2 REQUIRED)
find_package(GIO REQUIRED)

find_package(Intl REQUIRED)

find_package(AnthyUnicode)
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
        src/lib/logic/languageplugininterface.h
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
        plugins/westernsupport/spellpredictworker.cpp
        plugins/westernsupport/spellpredictworker.h
        plugins/westernsupport/westernlanguagefeatures.cpp
        plugins/westernsupport/westernlanguagefeatures.h
        plugins/westernsupport/westernlanguagesplugin.cpp
        plugins/westernsupport/westernlanguagesplugin.h
        plugins/westernsupport/westernsupport.cpp
        plugins/westernsupport/westernsupport.h
        plugins/westernsupport/westernsupport_global.h)

# TODO install logic/languageplugininterface.h and logic/abstractplugininterface.h as HEADERS

set(maliit-keyboard-libraries Qt${QT_VERSION_MAJOR}::Core)
set(maliit-keyboard-definitions HUNSPELL_DICT_PATH="${HUNSPELL_DICT_PATH}"
        MALIIT_GETTEXT_PACKAGE="${PROJECT_NAME}"
        MALIIT_LOCALE_DIR="${CMAKE_INSTALL_LOCALEDIR}/locale"
        MALIIT_PLUGINS_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_PLUGINS_DATA_DIR}"
        MALIIT_KEYBOARD_QML_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_KEYBOARD_QML_DIR}"
        MALIIT_KEYBOARD_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_KEYBOARD_DATA_DIR}"
        MALIIT_KEYBOARD_LANGUAGES_DIR="${CMAKE_INSTALL_PREFIX}/${MALIIT_KEYBOARD_LANGUAGES_DIR}")
set(maliit-keyboard-include-dirs)

if(enable-hunspell)
    find_package(Hunspell REQUIRED)
    list(APPEND maliit-keyboard-definitions HAVE_HUNSPELL)
    list(APPEND maliit-keyboard-libraries ${Hunspell_LIBRARIES})
    list(APPEND maliit-keyboard-include-dirs ${Hunspell_INCLUDE_DIRS})
endif()

add_library(maliit-keyboard-lib STATIC ${MALIIT_KEYBOARD_LIB_SOURCES})
target_link_libraries(maliit-keyboard-lib Qt${QT_VERSION_MAJOR}::Core Maliit::Plugins)
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
target_link_libraries(gsettings-qt Qt${QT_VERSION_MAJOR}::Core ${GIO_LIBRARIES})
target_include_directories(gsettings-qt PUBLIC ${GIO_INCLUDE_DIRS} src/3rdparty)
target_compile_definitions(gsettings-qt PRIVATE -DQT_NO_KEYWORDS)

set(MALIIT_KEYBOARD_COMMON_SOURCES
        src/plugin/editor.cpp
        src/plugin/editor.h
        src/plugin/feedback.cpp
        src/plugin/feedback.h
        src/plugin/gettext.cpp
        src/plugin/gettext.h
        src/plugin/updatenotifier.cpp
        src/plugin/updatenotifier.h
        src/plugin/inputmethod.cpp
        src/plugin/inputmethod.h
        src/plugin/inputmethod_p.h
        src/plugin/keyboardgeometry.cpp
        src/plugin/keyboardgeometry.h
        src/plugin/keyboardsettings.cpp
        src/plugin/keyboardsettings.h
        src/plugin/device.cpp
        src/plugin/device.h)

add_library(maliit-keyboard-common STATIC ${MALIIT_KEYBOARD_COMMON_SOURCES})
target_link_libraries(maliit-keyboard-common Qt${QT_VERSION_MAJOR}::DBus Qt${QT_VERSION_MAJOR}::QuickControls2 Maliit::Plugins maliit-keyboard-lib maliit-keyboard-view gsettings-qt Qt${QT_VERSION_MAJOR}::Multimedia ${Intl_LIBRARIES})
if (BUILD_WITH_QT6)
    target_link_libraries(maliit-keyboard-common Qt6::Core5Compat)
endif()
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
if (BUILD_WITH_QT6)
    target_link_libraries(westernsupport Qt6::Core5Compat)
endif()
target_include_directories(westernsupport PUBLIC src/lib/logic plugins/westernsupport ${maliit-keyboard-include-dirs})
target_compile_definitions(westernsupport PRIVATE ${maliit-keyboard-definitions})

function(language_plugin _language _full_language)
    # To support layout style variations such as en@dv we need to avoid using
    # the @ character in variables, so split and replace it with _
    set(_target ${_language})
    if(${_language} MATCHES "^(.*)\@(.*)$")
        set(_target ${CMAKE_MATCH_1}_${CMAKE_MATCH_2})
    endif()

    set(PLUGIN_SOURCES
            plugins/${_language}/src/${_full_language}plugin.h
            plugins/${_language}/src/${_full_language}plugin.json)
    add_library(${_target}plugin MODULE ${PLUGIN_SOURCES})
    set_target_properties(${_target}plugin PROPERTIES OUTPUT_NAME ${_language}plugin)
    target_link_libraries(${_target}plugin maliit-keyboard-common westernsupport)
    install(FILES plugins/${_language}/qml/Keyboard_${_language}.qml
            plugins/${_language}/qml/Keyboard_${_language}_email.qml
            plugins/${_language}/qml/Keyboard_${_language}_url.qml
            plugins/${_language}/qml/Keyboard_${_language}_url_search.qml
            DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    install(TARGETS ${_target}plugin
            LIBRARY DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    foreach(_file IN LISTS ARGN)
        install(FILES plugins/${_language}/${_file}
                DESTINATION ${MALIIT_KEYBOARD_LANGUAGES_DIR}/${_language})
    endforeach()
endfunction()

function(abstract_language_plugin _language _full_language)
    set(options ABSTRACT_LANGUAGE_PLUGIN)
    set(oneValueArgs PLUGIN_DIR LANGUAGE_FEATURES)
    set(multiValueArgs SOURCES LIBRARIES INCLUDE_DIRS DEFINITIONS FILES DIRECTORY)
    cmake_parse_arguments(abstract_language_plugin "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # To support layout style variations such as en@dv we need to avoid using
    # the @ character in variables, so split and replace it with _
    set(_target ${_language})
    if(${_language} MATCHES "^(.*)\@(.*)$")
        set(_target ${CMAKE_MATCH_1}_${CMAKE_MATCH_2})
    endif()

    set(_plugindir ${_language})
    if(NOT ${abstract_language_plugin_PLUGIN_DIR} EQUAL "")
        set(_plugindir ${abstract_language_plugin_PLUGIN_DIR})
    endif()
    set(PLUGIN_SOURCES
            plugins/${_plugindir}/src/${_full_language}plugin.h
            plugins/${_plugindir}/src/${_full_language}plugin.json)
    if(EXISTS ${CMAKE_SOURCE_DIR}/plugins/${_plugindir}/src/${_full_language}plugin.cpp)
        list(APPEND PLUGIN_SOURCES
            plugins/${_plugindir}/src/${_full_language}plugin.cpp)
    endif()
    if(NOT ${abstract_language_plugin_LANGUAGE_FEATURES} EQUAL "")
        list(APPEND PLUGIN_SOURCES
                plugins/${_plugindir}/src/${abstract_language_plugin_LANGUAGE_FEATURES}languagefeatures.cpp
                plugins/${_plugindir}/src/${abstract_language_plugin_LANGUAGE_FEATURES}languagefeatures.h)
    elseif(EXISTS ${CMAKE_SOURCE_DIR}/plugins/${_plugindir}/src/${_full_language}languagefeatures.cpp)
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
    add_library(${_target}plugin MODULE ${PLUGIN_SOURCES})
    set_target_properties(${_target}plugin PROPERTIES OUTPUT_NAME ${_language}plugin)
    target_link_libraries(${_target}plugin Qt${QT_VERSION_MAJOR}::Core Maliit::Plugins maliit-keyboard-common ${abstract_language_plugin_LIBRARIES})
    target_include_directories(${_target}plugin PRIVATE src/lib/logic ${abstract_language_plugin_INCLUDE_DIRS})
    target_compile_definitions(${_target}plugin PRIVATE ${abstract_language_plugin_DEFINITIONS})
    install(TARGETS ${_target}plugin
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
endfunction()

language_plugin(ar arabic)
language_plugin(az azerbaijani)
language_plugin(be belarusian)
language_plugin(bg bulgarian)
language_plugin(bs bosnian)
language_plugin(ca catalan src/overrides.csv)
language_plugin(cs czech)
language_plugin(da danish src/overrides.csv)
language_plugin(de german)
language_plugin(el greek)
language_plugin(en english src/overrides.csv)
language_plugin(eo esperanto)
language_plugin(es spanish)
language_plugin(fa persian qml/Keyboard_symbols_fa.qml)
language_plugin(fi finnish)
language_plugin(fr french src/overrides.csv)
language_plugin(gd gaelic)
language_plugin(he hebrew src/overrides.csv)
language_plugin(hr croatian)
language_plugin(hu hungarian)
language_plugin(is icelandic)
language_plugin(it italian src/overrides.csv)
language_plugin(lt lithuanian)
language_plugin(lv latvian)
language_plugin(mk macedonian)
language_plugin(nb norwegian)
language_plugin(nl dutch src/overrides.csv)
language_plugin(pl polish)
language_plugin(pt portuguese src/overrides.csv)
language_plugin(ro romanian)
language_plugin(ru russian)
language_plugin(sl slovenian)
language_plugin(sr serbian)
language_plugin(sv swedish src/overrides.csv)
language_plugin(tr turkish)
language_plugin(uk ukrainian)

abstract_language_plugin(en@dv englishdvorak
    FILES src/overrides.csv
    LIBRARIES westernsupport)
abstract_language_plugin(fr-ch french LIBRARIES westernsupport)
abstract_language_plugin(th thai DIRECTORY qml/keys)

if(AnthyUnicode_FOUND)
    abstract_language_plugin(ja japanese ABSTRACT_LANGUAGE_PLUGIN
            SOURCES anthyadapter.cpp anthyadapter.h
            LIBRARIES ${AnthyUnicode_LIBRARIES}
            INCLUDE_DIRS ${AnthyUnicode_INCLUDE_DIRS}
            DIRECTORY qml/keys)
elseif(Anthy_FOUND)
    abstract_language_plugin(ja japanese ABSTRACT_LANGUAGE_PLUGIN
            SOURCES anthyadapter.cpp anthyadapter.h
            LIBRARIES ${Anthy_LIBRARIES}
            INCLUDE_DIRS ${Anthy_INCLUDE_DIRS}
            DIRECTORY qml/keys)
endif()

abstract_language_plugin(ko korean
  LIBRARIES westernsupport
  DIRECTORY qml/keys)

if(Pinyin_FOUND)
    abstract_language_plugin(zh-hans pinyin ABSTRACT_LANGUAGE_PLUGIN
            PLUGIN_DIR pinyin
            LANGUAGE_FEATURES chinese
            SOURCES pinyinadapter.cpp pinyinadapter.h
            LIBRARIES ${Pinyin_LIBRARIES}
            INCLUDE_DIRS ${Pinyin_INCLUDE_DIRS}
            DEFINITIONS PINYIN_DATA_DIR="${Pinyin_DATA_DIR}")
endif()
if(Chewing_FOUND)
    abstract_language_plugin(zh-hant chewing ABSTRACT_LANGUAGE_PLUGIN
            PLUGIN_DIR chewing
            SOURCES chewingadapter.cpp chewingadapter.h
            LIBRARIES ${Chewing_LIBRARIES}
            INCLUDE_DIRS ${Chewing_INCLUDE_DIRS})
endif()

# Installation

install(FILES COPYING COPYING.BSD README.md
        DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/maliit-keyboard)

install(TARGETS maliit-keyboard-plugin maliit-keyboard
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/maliit${MALIIT_SUFFIX}/plugins
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY qml/keys qml/languages
        DESTINATION ${MALIIT_KEYBOARD_QML_DIR})
install(FILES qml/ActionsToolbar.qml qml/FloatingActions.qml qml/Keyboard.qml qml/KeyboardContainer.qml qml/WordRibbon.qml
        DESTINATION ${MALIIT_KEYBOARD_QML_DIR})

install(DIRECTORY data/devices
        DESTINATION ${MALIIT_KEYBOARD_DATA_DIR})
install(FILES data/schemas/org.maliit.keyboard.maliit.gschema.xml
        DESTINATION share/glib-2.0/schemas/)

install(FILES com.github.maliit.keyboard.desktop
        DESTINATION share/applications)
install(FILES com.github.maliit.keyboard.metainfo.xml
        DESTINATION share/metainfo)

# Translations
add_subdirectory(po)

# TODO add all tests
if(enable-tests)
    enable_testing()

    find_package(Qt${QT_VERSION_MAJOR}Test)

    add_library(test-utils STATIC
            tests/unittests/common/inputmethodhostprobe.cpp
            tests/unittests/common/inputmethodhostprobe.h
            tests/unittests/common/utils.cpp
            tests/unittests/common/utils.h)

    target_link_libraries(test-utils PUBLIC Maliit::Plugins Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test westernsupport)
    target_include_directories(test-utils PUBLIC tests/unittests tests/unittests/common src src/lib)

    function(create_test name)
        set(_extra_sources ${ARGV})
        list(REMOVE_AT _extra_sources 0)

        add_executable(${name}
                tests/unittests/${name}/${name}.cpp
                ${_extra_sources})

        target_link_libraries(${name} test-utils maliit-keyboard-common ${_libs})

        add_test(${name} ${name})

        set(test_targets ${test_targets} ${name} PARENT_SCOPE)
    endfunction()

    create_test(ut_languagefeatures)
    create_test(ut_repeat-backspace
            tests/unittests/common/wordengineprobe.cpp
            tests/unittests/common/wordengineprobe.h)
    create_test(ut_editor
            tests/unittests/ut_editor/wordengineprobe.cpp
            tests/unittests/ut_editor/wordengineprobe.h)
    #create_test(ut_preedit-string)
    create_test(ut_text)
    create_test(ut_word-candidates
            tests/unittests/ut_word-candidates/wordengineprobe.cpp
            tests/unittests/ut_word-candidates/wordengineprobe.h)
    create_test(ut_wordengine)

    set_property(TEST ${test_targets} PROPERTY ENVIRONMENT
            MALIIT_PLUGINS_DATADIR=${CMAKE_SOURCE_DIR}/data)

endif()
