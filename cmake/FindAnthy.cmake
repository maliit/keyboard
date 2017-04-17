include(FeatureSummary)
set_package_properties(anthy PROPERTIES
        URL "http://sourceforge.jp/projects/anthy/"
        DESCRIPTION "Anthy provides the library to input Japanese on the applications.")

find_package(PkgConfig)
pkg_check_modules(PC_ANTHY QUIET anthy)

find_library(ANTHY_LIBRARIES NAMES anthy ${PC_ANTHY_LIBRARIES} HINTS ${PC_ANTHY_LIBRARY_DIRS})
find_path(ANTHY_INCLUDE_DIRS anthy/anthy.h HINTS ${PC_ANTHY_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ANTHY DEFAULT_MSG ANTHY_LIBRARIES ANTHY_INCLUDE_DIRS)
mark_as_advanced(ANTHY_INCLUDE_DIRS ANTHY_LIBRARIES)