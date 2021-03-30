include(FeatureSummary)
set_package_properties(anthy PROPERTIES
        URL "http://sourceforge.jp/projects/anthy/"
        DESCRIPTION "Anthy provides the library to input Japanese on the applications.")

find_package(PkgConfig)
pkg_check_modules(PC_Anthy QUIET anthy)

find_library(Anthy_LIBRARIES NAMES anthy ${PC_Anthy_LIBRARIES} HINTS ${PC_Anthy_LIBRARY_DIRS})
find_path(Anthy_INCLUDE_DIRS anthy/anthy.h HINTS ${PC_Anthy_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Anthy DEFAULT_MSG Anthy_LIBRARIES Anthy_INCLUDE_DIRS)
mark_as_advanced(Anthy_INCLUDE_DIRS Anthy_LIBRARIES)