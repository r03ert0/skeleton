# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

set(CMAKE_PREFIX_PATH /Applications/_Sci/cgal-github/Installation/lib/cmake/CGAL)
set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 3.1...3.14)
project( skeleton )

find_package(CGAL REQUIRED QUIET OPTIONAL_COMPONENTS Core )
include( ${CGAL_USE_FILE} )

find_package(Eigen3 3.0.91) 
include( ${EIGEN3_USE_FILE} )

create_single_source_cgal_program( "skeleton.cpp" )
