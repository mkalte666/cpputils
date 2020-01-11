# funcForceStrictOptions.cmake
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

function(enableStrictOptions target)
  target_compile_options(
    ${target}
    PRIVATE
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
      -Wall
      -Werror
      -pedantic-errors
      -pedantic
      -Wextra
      -Wconversion
      -Wold-style-cast
      -Wuninitialized
      -Wunreachable-code
      -Wshadow
      -Wfloat-equal
      -Weffc++
      -Wsign-conversion>
      $<$<CXX_COMPILER_ID:MSVC>:
      /W4
      /WX>)
endfunction()
