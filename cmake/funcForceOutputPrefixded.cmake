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

function(forceOutputPrefixed prefixDirectory)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${prefixDirectory}/lib PARENT_SCOPE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${prefixDirectory}/lib PARENT_SCOPE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${prefixDirectory}/bin PARENT_SCOPE)
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} PARENT_SCOPE)
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} PARENT_SCOPE)
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} PARENT_SCOPE)
	endforeach()
endfunction()