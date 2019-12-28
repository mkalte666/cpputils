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
set(_cpputils_uuid_sources ${utilsIncludeDir}/uuid/uuid.h uuid.cpp)

add_library(_cpputils_uuid STATIC)
target_include_directories(_cpputils_uuid PUBLIC ${utilsIncludeDir}/uuid)
target_sources(_cpputils_uuid PRIVATE ${_cpputils_uuid_sources})

target_link_libraries(_cpputils_uuid PUBLIC cpputils::serialization)

add_library(cpputils::uuid ALIAS _cpputils_uuid)

if(${cpputilsBuildDocs})
  doxygen_add_docs(cpputils_uuid_doc
                   ${_cpputils_uuid_sources}
                   COMMENT
                   "PtrClass Documentaion")
  add_dependencies(_cpputils_uuid cpputils_uuid_doc)
endif()

enablestrictoptions(_cpputils_uuid)
