include(FeatureSummary)
set_package_properties(PRESAGE PROPERTIES
        URL "http://presage.sourceforge.net/"
        DESCRIPTION "Presage is an intelligent predictive text entry platform.")

find_library(PRESAGE_LIBRARIES presage)
find_path(PRESAGE_INCLUDE_DIRS presage.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PRESAGE DEFAULT_MSG PRESAGE_LIBRARIES PRESAGE_INCLUDE_DIRS)
mark_as_advanced(PRESAGE_INCLUDE_DIRS PRESAGE_LIBRARIES)

function(add_ngram)
    # Parse arguments
    set(oneValueArgs TEXT DATABASE)
    cmake_parse_arguments(ARGS "" "${oneValueArgs}" "" ${ARGN})

    if(ARGS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to add_ngram(): \"${ARGS_UNPARSED_ARGUMENTS}\"")
    endif()

    get_filename_component(_infile ${ARGS_TEXT} ABSOLUTE)
    set(_database "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_DATABASE}")

    set_source_files_properties(${_database} GENERATED)

    add_custom_command(OUTPUT "${_database}"
            COMMAND rm -f ${_database}
            COMMAND text2ngram -n 1 -l -f sqlite -o ${_database} ${_infile}
            COMMAND text2ngram -n 2 -l -f sqlite -o ${_database} ${_infile}
            COMMAND text2ngram -n 3 -l -f sqlite -o ${_database} ${_infile}
            DEPENDS ${_infile} VERBATIM)
endfunction()
