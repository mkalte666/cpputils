#    CMakeLists.txt 
#
#    Copyright (C) 2019  Malte Kieﬂling
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 17)

# this is for the find package n stuff.
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

project(cpputils)

set (utilsIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)


