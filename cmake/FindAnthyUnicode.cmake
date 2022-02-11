include(FeatureSummary)
set_package_properties(anthyunicode PROPERTIES
        URL "https://github.com/fujiwarat/anthy-unicode"
        DESCRIPTION "Anthy provides the library to input Japanese on the applications.")

find_package(PkgConfig)
pkg_check_modules(PC_AnthyUnicode QUIET anthy-unicode)

find_library(AnthyUnicode_LIBRARIES NAMES anthy-unicode ${PC_AnthyUnicode_LIBRARIES} HINTS ${PC_AnthyUnicode_LIBRARY_DIRS})
find_path(AnthyUnicode_INCLUDE_DIRS anthy/anthy.h HINTS ${PC_AnthyUnicode_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AnthyUnicode DEFAULT_MSG AnthyUnicode_LIBRARIES AnthyUnicode_INCLUDE_DIRS)
mark_as_advanced(AnthyUnicode_INCLUDE_DIRS AnthyUnicode_LIBRARIES)
