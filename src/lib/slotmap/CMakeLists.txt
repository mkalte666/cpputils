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
set(_cpputils_slotmap_sources ${utilsIncludeDir}/slotmap/slotmap.h)

add_library(_cpputils_slotmap INTERFACE)
target_include_directories(_cpputils_slotmap
                           INTERFACE ${utilsIncludeDir}/slotmap)
target_sources(_cpputils_slotmap INTERFACE ${_cpputils_slotmap_sources})

add_library(cpputils::slotmap ALIAS _cpputils_slotmap)

if(${cpputilsBuildDocs})
  doxygen_add_docs(cpputils_slotmap_doc ${_cpputils_slotmap_sources} COMMENT
                   "PtrClass Documentaion")
  add_dependencies(_cpputils_slotmap cpputils_slotmap_doc)
endif()
