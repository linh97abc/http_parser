
set (HTTP_PARSER_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/http_parser")
file(MAKE_DIRECTORY ${HTTP_PARSER_GENERATED_DIR})

if(NOT DEFINED HTTP_PARSER_SRC_ROOT_FOLDER)
get_filename_component(HTTP_PARSER_SRC_ROOT_FOLDER
${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
endif()



find_path(HTTP_PARSER_INCLUDE_DIRS
http_parser.h
PATHS ${HTTP_PARSER_SRC_ROOT_FOLDER}/include
NO_CMAKE_FIND_ROOT_PATH
)
mark_as_advanced(HTTP_PARSER_INCLUDE_DIRS)

set(HTTP_PARSER_SRCS 
${CMAKE_CURRENT_LIST_DIR}/src/http_parser.c
${CMAKE_CURRENT_LIST_DIR}/src/wic.c
${CMAKE_CURRENT_LIST_DIR}/src/webutil.c)

add_library(http_parser STATIC EXCLUDE_FROM_ALL ${HTTP_PARSER_SRCS})
# target_compile_features(http_parser PUBLIC c_std_11)

add_library(HttpParser::HttpParser ALIAS http_parser)

target_include_directories(http_parser PUBLIC ${HTTP_PARSER_INCLUDE_DIRS})
target_include_directories(http_parser PUBLIC ${HTTP_PARSER_GENERATED_DIR})




function(HTTP_PARSER_GENERATE_CPP)
  cmake_parse_arguments(HTTP_PARSER_GENERATE_CPP "" "TARGET" "" ${ARGN})

  set(HTTP_PARSER_GENERATOR_EXECUTABLE ${HTTP_PARSER_SRC_ROOT_FOLDER}/scripts/web_minify.py)
  set(HDRS ${HTTP_PARSER_GENERATE_CPP_UNPARSED_ARGUMENTS})

  set(output_file ${HTTP_PARSER_GENERATED_DIR}/webfs.inc.c ${HTTP_PARSER_GENERATED_DIR}/webfs.inc)
  set(web_file)
  foreach(FIL ${HTTP_PARSER_GENERATE_CPP_UNPARSED_ARGUMENTS})
      get_filename_component(FIL_WE ${FIL} NAME_WLE)
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      list(APPEND web_file ${ABS_FIL})
  endforeach()
  
  # set_source_files_properties(${output_file} PROPERTIES GENERATED TRUE)
  # target_sources(http_parser PRIVATE ${output_file})
  add_library(${HTTP_PARSER_GENERATE_CPP_TARGET} STATIC EXCLUDE_FROM_ALL ${output_file})
  target_link_libraries(${HTTP_PARSER_GENERATE_CPP_TARGET} http_parser)

  add_custom_command(
    OUTPUT ${output_file}
    COMMAND ${CMAKE_COMMAND} -E echo "Generating http_parser files"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${HTTP_PARSER_GENERATED_DIR}
    COMMAND ${CMAKE_COMMAND} -E env python ${HTTP_PARSER_GENERATOR_EXECUTABLE} --output_dir=${HTTP_PARSER_GENERATED_DIR} ${HDRS}
    DEPENDS ${HTTP_PARSER_GENERATOR_EXECUTABLE} ${web_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} 
    COMMENT "Generating webfs.inc"
  )




endfunction()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HttpParser DEFAULT_MSG
  HTTP_PARSER_INCLUDE_DIRS
  HTTP_PARSER_SRCS
  )
