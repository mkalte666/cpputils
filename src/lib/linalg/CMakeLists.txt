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
#
set(_cpputils_linalg_sources ${utilsIncludeDir}/linalg/vec.h
                             ${utilsIncludeDir}/linalg/quaternion.h)

add_library(_cpputils_linalg INTERFACE)
target_include_directories(_cpputils_linalg INTERFACE ${utilsIncludeDir}/linalg)
target_sources(_cpputils_linalg INTERFACE ${_cpputils_linalg_sources})

add_library(cpputils::linalg ALIAS _cpputils_linalg)

if(${cpputilsBuildDocs})
  doxygen_add_docs(cpputils_linalg_doc ${_cpputils_linalg_sources} COMMENT
                   "LinAlg Documentaion")
  add_dependencies(_cpputils_linalg cpputils_linalg_doc)
endif()
