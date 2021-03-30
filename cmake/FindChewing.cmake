include(FeatureSummary)
set_package_properties(Chewing PROPERTIES
        URL "http://chewing.csie.net/"
        DESCRIPTION "Intelligent Chinese phonetic input method library")

find_package(PkgConfig)
pkg_check_modules(PC_Chewing QUIET chewing)

find_library(Chewing_LIBRARIES NAMES chewing ${PC_Chewing_LIBRARIES} HINTS ${PC_Chewing_LIBRARY_DIRS})
find_path(Chewing_INCLUDE_DIRS chewing.h HINTS ${PC_Chewing_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Chewing DEFAULT_MSG Chewing_LIBRARIES Chewing_INCLUDE_DIRS)
mark_as_advanced(Chewing_INCLUDE_DIRS Chewing_LIBRARIES)
