include(FeatureSummary)
set_package_properties(Presage PROPERTIES
        URL "http://presage.sourceforge.net/"
        DESCRIPTION "Presage is an intelligent predictive text entry platform.")

find_library(Presage_LIBRARIES presage)
find_path(Presage_INCLUDE_DIRS presage.h)
find_program(Presage_TEXT2NGRAM text2ngram)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Presage DEFAULT_MSG Presage_LIBRARIES Presage_INCLUDE_DIRS)
mark_as_advanced(Presage_INCLUDE_DIRS Presage_LIBRARIES)

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
            COMMAND ${Presage_TEXT2NGRAM} -n 1 -l -f sqlite -o ${_database} ${_infile}
            COMMAND ${Presage_TEXT2NGRAM} -n 2 -l -f sqlite -o ${_database} ${_infile}
            COMMAND ${Presage_TEXT2NGRAM} -n 3 -l -f sqlite -o ${_database} ${_infile}
            DEPENDS ${_infile} VERBATIM)
endfunction()
