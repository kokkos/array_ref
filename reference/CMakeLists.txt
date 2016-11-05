################################################################################
# Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
################################################################################

cmake_minimum_required(VERSION 3.1) 

project(mdspan CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall")

include_directories(${mdspan_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS include/array_ref include/*.hpp)
source_group("Headers" FILES ${HEADERS})

set(TESTS
    test_array_ref_basics
    test_dims_dynamic_extent
    test_dims_index_into_dynamic_dims
    test_dims_meta_interfaces
    test_dims_rt_interfaces
    test_layout_mapping_left_basics
    test_layout_mapping_left_padding
    test_layout_mapping_left_stepping
    test_layout_mapping_left_stepping_padding
    test_layout_mapping_right_basics
    test_layout_mapping_right_padding
    test_layout_mapping_right_stepping
    test_layout_mapping_right_stepping_padding
    test_meta_count_dynamic_dims
    test_meta_count_integral_range_slice_specifiers
    test_meta_extract_dimensions
    test_meta_is_slice_specifier
    test_meta_make_dynamic_dims_array
    test_meta_make_filled_dims
    test_meta_pack_is_integral
   )

add_custom_target(array_ref SOURCES ${HEADERS})

foreach(TEST ${TESTS})
  add_executable(${TEST} ${mdspan_SOURCE_DIR}/tests/${TEST}.cpp)
endforeach()

enable_testing()

foreach(TEST ${TESTS})
  add_test(${TEST} ${TEST})
endforeach()

