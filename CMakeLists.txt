# CMakeLists.txt
#
# Copyright (C) 2019  Malte Kießling
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or any later version.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.

# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <https://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

# disable in-source build. found at
# https://stackoverflow.com/questions/1208681/with-cmake-how-would-you-disable-
# in-source-builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# this is for the find package n stuff.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)

project(cpputils)

set(utilsIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(thridpartyDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
option(cpputilsBuildExamples OFF)
option(cpputilsBuildDocs OFF)
if(${cpputilsBuildDocs})
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
  find_package(Doxygen REQUIRED)
endif()

include(cmake/funcForceOutputPrefixded.cmake)
include(cmake/funcRuntimeCopy.cmake)
include(cmake/funcEnableStrictOptions.cmake)
include(cmake/funcEnableClangTidy.cmake)

add_subdirectory(src)


