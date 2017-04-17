include(FeatureSummary)
set_package_properties(HUNSPELL PROPERTIES
        URL "http://hunspell.sourceforge.net/"
        DESCRIPTION "Hunspell is a spell checker library.")


find_package(PkgConfig)
pkg_check_modules(PC_HUNSPELL QUIET hunspell)

find_library(HUNSPELL_LIBRARIES NAMES hunspell ${PC_HUNSPELL_LIBRARIES} HINTS ${PC_HUNSPELL_LIBRARY_DIRS})
find_path(HUNSPELL_INCLUDE_DIRS hunspell/hunspell.hxx HINTS ${PC_HUNSPELL_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HUNSPELL DEFAULT_MSG HUNSPELL_LIBRARIES HUNSPELL_INCLUDE_DIRS)
mark_as_advanced(HUNSPELL_INCLUDE_DIRS HUNSPELL_LIBRARIES)