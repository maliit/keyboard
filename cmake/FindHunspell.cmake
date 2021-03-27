include(FeatureSummary)
set_package_properties(Hunspell PROPERTIES
        URL "http://hunspell.sourceforge.net/"
        DESCRIPTION "Hunspell is a spell checker library.")


find_package(PkgConfig)
pkg_check_modules(PC_Hunspell QUIET hunspell)

find_library(Hunspell_LIBRARIES NAMES hunspell ${PC_Hunspell_LIBRARIES} HINTS ${PC_Hunspell_LIBRARY_DIRS})
find_path(Hunspell_INCLUDE_DIRS hunspell/hunspell.hxx HINTS ${PC_Hunspell_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Hunspell DEFAULT_MSG Hunspell_LIBRARIES Hunspell_INCLUDE_DIRS)
mark_as_advanced(Hunspell_INCLUDE_DIRS Hunspell_LIBRARIES)