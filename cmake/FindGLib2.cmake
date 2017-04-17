include(FeatureSummary)
set_package_properties(GLIB2 PROPERTIES
        URL "https://wiki.gnome.org/Projects/GLib"
        DESCRIPTION "GLib provides the core application building blocks for libraries and applications written in C")

find_package(PkgConfig)
pkg_check_modules(PC_GLIB2 glib-2.0 QUIET)

set(GLIB2_INCLUDE_DIRS ${PC_GLIB2_INCLUDE_DIRS})

foreach(COMP ${PC_GLIB2_LIBRARIES})
    find_library(GLIB2_${COMP} NAMES ${COMP} HINTS ${PC_GLIB2_LIBRARY_DIRS})
    list(APPEND GLIB2_LIBRARIES ${GLIB2_${COMP}})
endforeach()

execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable glib_genmarshal glib-2.0
        OUTPUT_VARIABLE GLIB2_GENMARHSAL_EXECUTABLE)

string(REGEX REPLACE "[\r\n]" " " GLIB2_GENMARHSAL_EXECUTABLE "${GLIB2_GENMARHSAL_EXECUTABLE}")
string(REGEX REPLACE " +$" "" GLIB2_GENMARHSAL_EXECUTABLE "${GLIB2_GENMARHSAL_EXECUTABLE}")

# handle the QUIETLY and REQUIRED arguments and set GLIB2_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLIB2 DEFAULT_MSG GLIB2_LIBRARIES GLIB2_INCLUDE_DIRS)

mark_as_advanced(GLIB2_INCLUDE_DIRS GLIB2_LIBRARIES)

if(PC_GLIB2_FOUND AND NOT TARGET GLib2::GLib)
    add_library(GLib2::GLib INTERFACE IMPORTED)

    set_property(TARGET GLib2::GLib PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GLIB2_INCLUDE_DIRS})
    set_property(TARGET GLib2::GLib PROPERTY INTERFACE_LINK_LIBRARIES ${GLIB2_LIBRARIES})
endif()

function(GLIB2_ADD_MARSHAL _sources _marshallist)
    get_filename_component(_infile ${_marshallist} ABSOLUTE)
    get_filename_component(_basename ${_marshallist} NAME_WE)
    set(_header "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h")
    set(_body   "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c")

    get_source_file_property(_prefix ${_marshallist} PREFIX)
    if(_prefix)
        set(_params ${_params} --prefix ${_prefix})
    endif()

    add_custom_command(OUTPUT "${_header}"
            COMMAND ${GLIB2_GENMARHSAL_EXECUTABLE} --header ${_params} ${_infile} > ${_header}
            DEPENDS ${_infile} VERBATIM)
    add_custom_command(OUTPUT "${_body}"
            COMMAND ${GLIB2_GENMARHSAL_EXECUTABLE} --body ${_params} ${_infile} > ${_body}
            DEPENDS ${_infile} VERBATIM)

    list(APPEND ${_sources} "${_body}" "${_header}")
    set(${_sources} ${${_sources}} PARENT_SCOPE)
endfunction()
