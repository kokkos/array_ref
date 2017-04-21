################################################################################
# Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
# Copyright (c) 2014 Thomas Heller (mdspan_option() CMake function)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
################################################################################

cmake_minimum_required(VERSION 3.1) 

project(mdspan CXX)

################################################################################

include(CMakeParseArguments)

function(mdspan_option _option _type _description _default)
  set(_named_params ADVANCED)
  set(_one_value_args CATEGORY)
  set(_multi_value_args)

  cmake_parse_arguments(_parsed_option_args
    "${_named_params}" "${_one_value_args}" "${_multi_value_args}" ${ARGN})

  if(NOT DEFINED ${_option})
    set(${_option} ${_default} CACHE ${_type} "${_description}")
    if(_parsed_option_args_ADVANCED)
      mark_as_advanced(${_option})
    endif()
  else()
    set_property(CACHE "${_option}" PROPERTY HELPSTRING "${_description}")
    set_property(CACHE "${_option}" PROPERTY TYPE "${_type}")
  endif()
endfunction()

################################################################################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

mdspan_option(MDSPAN_WARNINGS
  BOOL
  "If enabled, mdspan tests and benchmarks will be built with warnings enabled."
  ON)

if(MDSPAN_WARNINGS)
  if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W3")
  endif()
endif()

include_directories(${mdspan_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS include/mdspan include/*.hpp)
source_group("Headers" FILES ${HEADERS})
add_custom_target(mdspan SOURCES ${HEADERS})

set(TESTS
    test_mdspan_basics
    test_dims_dynamic_extent
    test_dims_index_into_dynamic_dims
    test_dims_filter_initialize_dynamic_dims_array
    test_dims_meta_interfaces
    test_dims_runtime_interfaces
    test_integer_sequence
    test_layout_mapping_left_basics
    test_layout_mapping_left_padding
    test_layout_mapping_left_stepping
    test_layout_mapping_left_stepping_padding
    test_layout_mapping_right_basics
    test_layout_mapping_right_padding
    test_layout_mapping_right_stepping
    test_layout_mapping_right_stepping_padding
    test_layout_mapping_regular_precomputed_strides_basics
    test_layout_mapping_regular_precomputed_strides_left_basics
    test_meta_count_dynamic_dims
    test_meta_count_integral_range_slice_specifiers
    test_meta_extract_dimensions
    test_meta_is_slice_specifier
    test_meta_make_dynamic_dims_array
    test_meta_make_dynamic_dims_indices
    test_meta_make_filled_dims
    test_meta_push_front_and_push_back
    test_meta_is_integral_pack
    test_meta_type_list
   )

foreach(TEST ${TESTS})
  add_executable(${TEST} ${mdspan_SOURCE_DIR}/tests/${TEST}.cpp)
endforeach()

enable_testing()

foreach(TEST ${TESTS})
  add_test(${TEST} ${TEST})
endforeach()

