include(FeatureSummary)
set_package_properties(CHEWING PROPERTIES
        URL "http://chewing.csie.net/"
        DESCRIPTION "Intelligent Chinese phonetic input method library")

find_package(PkgConfig)
pkg_check_modules(PC_CHEWING QUIET chewing)

find_library(CHEWING_LIBRARIES NAMES chewing ${PC_CHEWING_LIBRARIES} HINTS ${PC_CHEWING_LIBRARY_DIRS})
find_path(CHEWING_INCLUDE_DIRS chewing.h HINTS ${PC_CHEWING_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CHEWING DEFAULT_MSG CHEWING_LIBRARIES CHEWING_INCLUDE_DIRS)
mark_as_advanced(CHEWING_INCLUDE_DIRS CHEWING_LIBRARIES)
