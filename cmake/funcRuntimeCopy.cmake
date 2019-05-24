#    CMakeLists.txt 
#
#    Copyright (C) 2019  Malte Kie�ling
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

function(runtimeCopy fileList)
	foreach(filename ${fileList})
		get_filename_component(filename_only ${filename} NAME)	
		message("Copying file required for runtime: ${filename}")
		configure_file(${filename} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${filename_only} COPYONLY)
	endforeach()
endfunction()

function(runtimeCopyWin32 fileList)
	if(WIN32)
		runtimeCopy(${fileList})
	endif()
endfunction()