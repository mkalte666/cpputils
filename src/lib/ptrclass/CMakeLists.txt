#    CMakeLists.txt 
#
#    Copyright (C) 2019  Malte Kießling
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
#
set(_cpputils_ptrclass_sources 
	${utilsIncludeDir}/ptrclass/ptrclass.h
	)

add_library(_cpputils_ptrclass INTERFACE)
target_include_directories(_cpputils_ptrclass INTERFACE ${utilsIncludeDir}/ptrclass)
target_sources(_cpputils_ptrclass INTERFACE ${_cpputils_ptrclass_sources})

add_library(cpputils::ptrclass ALIAS _cpputils_ptrclass)

if (${cpputilsBuildDocs})
doxygen_add_docs(cpputils_ptrclass_doc ${_cpputils_ptrclass_sources} COMMENT "PtrClass Documentaion")
add_dependencies(_cpputils_ptrclass cpputils_ptrclass_doc)
endif()