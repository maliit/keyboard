include(FeatureSummary)
set_package_properties(GLib2 PROPERTIES
        URL "https://wiki.gnome.org/Projects/GLib"
        DESCRIPTION "GLib provides the core application building blocks for libraries and applications written in C")

find_package(PkgConfig)
pkg_check_modules(PC_GLib2 glib-2.0 QUIET)

set(GLib2_INCLUDE_DIRS ${PC_GLib2_INCLUDE_DIRS})

foreach(COMP ${PC_GLib2_LIBRARIES})
    find_library(GLib2_${COMP} NAMES ${COMP} HINTS ${PC_GLib2_LIBRARY_DIRS})
    list(APPEND GLib2_LIBRARIES ${GLib2_${COMP}})
endforeach()

execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable glib_genmarshal glib-2.0
        OUTPUT_VARIABLE GLib2_GENMARHSAL_EXECUTABLE)

string(REGEX REPLACE "[\r\n]" " " GLib2_GENMARHSAL_EXECUTABLE "${GLib2_GENMARHSAL_EXECUTABLE}")
string(REGEX REPLACE " +$" "" GLib2_GENMARHSAL_EXECUTABLE "${GLib2_GENMARHSAL_EXECUTABLE}")

# handle the QUIETLY and REQUIRED arguments and set GLib2_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLib2 DEFAULT_MSG GLib2_LIBRARIES GLib2_INCLUDE_DIRS)

mark_as_advanced(GLib2_INCLUDE_DIRS GLib2_LIBRARIES)

if(PC_GLib2_FOUND AND NOT TARGET GLib2::GLib)
    add_library(GLib2::GLib INTERFACE IMPORTED)

    set_property(TARGET GLib2::GLib PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GLib2_INCLUDE_DIRS})
    set_property(TARGET GLib2::GLib PROPERTY INTERFACE_LINK_LIBRARIES ${GLib2_LIBRARIES})
endif()

function(GLib2_ADD_MARSHAL _sources _marshallist)
    get_filename_component(_infile ${_marshallist} ABSOLUTE)
    get_filename_component(_basename ${_marshallist} NAME_WE)
    set(_header "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h")
    set(_body   "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c")

    get_source_file_property(_prefix ${_marshallist} PREFIX)
    if(_prefix)
        set(_params ${_params} --prefix ${_prefix})
    endif()

    add_custom_command(OUTPUT "${_header}"
            COMMAND ${GLib2_GENMARHSAL_EXECUTABLE} --header ${_params} ${_infile} > ${_header}
            DEPENDS ${_infile} VERBATIM)
    add_custom_command(OUTPUT "${_body}"
            COMMAND ${GLib2_GENMARHSAL_EXECUTABLE} --body ${_params} ${_infile} > ${_body}
            DEPENDS ${_infile} VERBATIM)

    list(APPEND ${_sources} "${_body}" "${_header}")
    set(${_sources} ${${_sources}} PARENT_SCOPE)
endfunction()
