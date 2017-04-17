find_package(PkgConfig)
pkg_check_modules(PC_GIO gio-2.0 gio-unix-2.0 QUIET)

set(GIO_INCLUDE_DIRS ${PC_GIO_INCLUDE_DIRS})

foreach(COMP ${PC_GIO_LIBRARIES})
    find_library(GIO_${COMP} NAMES ${COMP} HINTS ${PC_GIO_LIBRARY_DIRS})
    list(APPEND GIO_LIBRARIES ${GIO_${COMP}})
endforeach()

execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable gdbus_codegen gio-2.0
        OUTPUT_VARIABLE GDBUS_CODEGEN_EXECUTABLE)

string(REGEX REPLACE "[\r\n]" " " GDBUS_CODEGEN_EXECUTABLE "${GDBUS_CODEGEN_EXECUTABLE}")
string(REGEX REPLACE " +$" "" GDBUS_CODEGEN_EXECUTABLE "${GDBUS_CODEGEN_EXECUTABLE}")

# handle the QUIETLY and REQUIRED arguments and set GLIB2_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GIO DEFAULT_MSG GIO_LIBRARIES GIO_INCLUDE_DIRS)

mark_as_advanced(GIO_INCLUDE_DIRS GIO_LIBRARIES)

if(PC_GIO_FOUND AND NOT TARGET GLib2::GIO)
    add_library(GLib2::GIO INTERFACE IMPORTED)

    set_property(TARGET GLib2::GIO PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GIO_INCLUDE_DIRS})
    set_property(TARGET GLib2::GIO PROPERTY INTERFACE_LINK_LIBRARIES ${GIO_LIBRARIES})
endif()

function(GDBUS_ADD_CODE _sources _interface)
    get_filename_component(_infile ${_interface} ABSOLUTE)
    get_filename_component(_basename ${_interface} NAME_WE)
    set(_output "${CMAKE_CURRENT_BINARY_DIR}/${_basename}")

    get_source_file_property(_interface_prefix ${_interface} INTERFACE_PREFIX)
    if(_interface_prefix)
        set(_params ${_params} --interface-prefix ${_interface_prefix})
    endif()

    get_source_file_property(_output_name ${_interface} OUTPUT_NAME)
    if(_output_name)
        set(_output "${CMAKE_CURRENT_BINARY_DIR}/${_output_name}")
    endif()

    get_source_file_property(_namespace ${_interface} NAMESPACE)
    if(_namespace)
        set(_params ${_params} --c-namespace ${_namespace})
    endif()

    get_source_file_property(_annotate ${_interface} ANNOTATE)
    if(_annotate)
        set(_params ${_params} --annotate ${_annotate})
    endif()

    get_filename_component(_output_directory ${_output} DIRECTORY)
    file(MAKE_DIRECTORY ${_output_directory})

    add_custom_command(OUTPUT "${_output}.c" "${_output}.h"
            COMMAND ${GDBUS_CODEGEN_EXECUTABLE} ${_params} --generate-c-code ${_output} ${_infile}
            DEPENDS ${_infile} VERBATIM)

    list(APPEND ${_sources} "${_output}.c" "${_output}.h")
    set(${_sources} ${${_sources}} PARENT_SCOPE)
endfunction()
