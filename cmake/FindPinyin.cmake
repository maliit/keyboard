include(FeatureSummary)
set_package_properties(Pinyin PROPERTIES
        URL "https://github.com/libpinyin/libpinyin"
        DESCRIPTION "Library to deal with pinyin")

find_package(PkgConfig)
pkg_check_modules(PC_Pinyin QUIET libpinyin)

find_library(Pinyin_LIBRARIES NAMES pinyin ${PC_Pinyin_LIBRARIES} HINTS ${PC_Pinyin_LIBRARY_DIRS})
find_path(Pinyin_INCLUDE_DIRS pinyin.h HINTS ${PC_Pinyin_INCLUDE_DIRS})

if(PC_Pinyin_FOUND)
        execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable pkgdatadir libpinyin
                OUTPUT_VARIABLE PC_Pinyin_DATA_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
        find_path(Pinyin_DATA_DIR bigram.db HINTS ${PC_Pinyin_DATA_DIR}/data)

        find_package(GLib2)
        list(APPEND Pinyin_LIBRARIES ${GLib2_LIBRARIES})
        list(APPEND Pinyin_INCLUDE_DIRS ${GLib2_INCLUDE_DIRS})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pinyin DEFAULT_MSG Pinyin_LIBRARIES Pinyin_INCLUDE_DIRS)
mark_as_advanced(Pinyin_INCLUDE_DIRS Pinyin_LIBRARIES)
