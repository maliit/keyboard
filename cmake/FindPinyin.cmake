include(FeatureSummary)
set_package_properties(PINYIN PROPERTIES
        URL "https://github.com/libpinyin/libpinyin"
        DESCRIPTION "Library to deal with pinyin")

find_package(PkgConfig)
pkg_check_modules(PC_PINYIN QUIET libpinyin)

find_library(PINYIN_LIBRARIES NAMES pinyin ${PC_PINYIN_LIBRARIES} HINTS ${PC_PINYIN_LIBRARY_DIRS})
find_path(PINYIN_INCLUDE_DIRS pinyin.h HINTS ${PC_PINYIN_INCLUDE_DIRS})

execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable pkgdatadir libpinyin
        OUTPUT_VARIABLE PC_PINYIN_DATA_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
find_path(PINYIN_DATA_DIR bigram.db HINTS ${PC_PINYIN_DATA_DIR}/data)

find_package(GLib2)
list(APPEND PINYIN_LIBRARIES ${GLIB2_LIBRARIES})
list(APPEND PINYIN_INCLUDE_DIRS ${GLIB2_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PINYIN DEFAULT_MSG PINYIN_LIBRARIES PINYIN_INCLUDE_DIRS)
mark_as_advanced(PINYIN_INCLUDE_DIRS PINYIN_LIBRARIES)
