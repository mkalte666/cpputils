# * Find SDL2 Find the SDL2 headers and libraries
#
# SDL2::SDL2_image - Imported target to use for building a library
# SDL2_IMAGE_IMAGE_FOUND - True if SDL2 was found. SDL2_IMAGE_IMAGE_DYNAMIC - If
# we found a DLL version of SDL (meaning you might want to copy a DLL from
# SDL2::SDL2_image)
#
# 2019 Malte Kießling <mkalte@mkalte.me>: Added support for SDL2_IMAGE_IMAGE
#
# Original Author: 2015 Ryan Pavlik <ryan.pavlik@gmail.com> <abiryan@ryand.net>
#
# Copyright Sensics, Inc. 2015. Distributed under the Boost Software License,
# Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# Set up architectures (for windows) and prefixes (for mingw builds)
if(WIN32)
  if(MINGW)
    include(MinGWSearchPathExtras OPTIONAL)
    if(MINGWSEARCH_TARGET_TRIPLE)
      set(SDL2_IMAGE_PREFIX ${MINGWSEARCH_TARGET_TRIPLE})
    endif()
  endif()
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL2_IMAGE_LIB_PATH_SUFFIX lib/x64)
    if(NOT MSVC AND NOT SDL2_IMAGE_PREFIX)
      set(SDL2_IMAGE_PREFIX x86_64-w64-mingw32)
    endif()
  else()
    set(SDL2_IMAGE_LIB_PATH_SUFFIX lib/x86)
    if(NOT MSVC AND NOT SDL2_IMAGE_PREFIX)
      set(SDL2_IMAGE_PREFIX i686-w64-mingw32)
    endif()
  endif()
endif()

if(SDL2_IMAGE_PREFIX)
  set(SDL2_IMAGE_ORIGPREFIXPATH ${CMAKE_PREFIX_PATH})
  if(SDL2_IMAGE_ROOT_DIR)
    list(APPEND CMAKE_PREFIX_PATH "${SDL2_IMAGE_ROOT_DIR}")
  endif()
  if(CMAKE_PREFIX_PATH)
    foreach(_prefix ${CMAKE_PREFIX_PATH})
      list(APPEND CMAKE_PREFIX_PATH "${_prefix}/${SDL2_IMAGE_PREFIX}")
    endforeach()
  endif()
  if(MINGWSEARCH_PREFIXES)
    list(APPEND CMAKE_PREFIX_PATH ${MINGWSEARCH_PREFIXES})
  endif()
endif()

# Invoke pkgconfig for hints
find_package(PkgConfig QUIET)
set(SDL2_IMAGE_INCLUDE_HINTS)
set(SDL2_IMAGE_LIB_HINTS)
if(PKG_CONFIG_FOUND)
  pkg_search_module(SDL2PC QUIET sdl2_image)
  if(SDL2PC_INCLUDE_DIRS)
    set(SDL2_IMAGE_INCLUDE_HINTS ${SDL2PC_INCLUDE_DIRS})
  endif()
  if(SDL2PC_LIBRARY_DIRS)
    set(SDL2_IMAGE_LIB_HINTS ${SDL2PC_LIBRARY_DIRS})
  endif()
endif()

include(FindPackageHandleStandardArgs)

find_library(
  SDL2_IMAGE_LIBRARY
  NAMES SDL2_image
  HINTS ${SDL2_IMAGE_LIB_HINTS}
  PATHS ${SDL2_IMAGE_ROOT_DIR} ENV SDL2DIR ENV SDL2IMAGEDIR
  PATH_SUFFIXES lib SDL2 ${SDL2_IMAGE_LIB_PATH_SUFFIX})

set(_SDL2_IMAGE_framework FALSE)
# Some special-casing if we've found/been given a framework. Handles whether
# we're given the library inside the framework or the framework itself.
if(APPLE AND "${SDL2_IMAGE_LIBRARY}" MATCHES "(/[^/]+)*.framework(/.*)?$")
  set(_SDL2_IMAGE_framework TRUE)
  set(SDL2_IMAGE_FRAMEWORK "${SDL2_IMAGE_LIBRARY}")
  # Move up in the directory tree as required to get the framework directory.
  while("${SDL2_IMAGE_FRAMEWORK}" MATCHES "(/[^/]+)*.framework(/.*)$"
        AND NOT "${SDL2_IMAGE_FRAMEWORK}" MATCHES "(/[^/]+)*.framework$")
    get_filename_component(SDL2_IMAGE_FRAMEWORK "${SDL2_IMAGE_FRAMEWORK}"
                           DIRECTORY)
  endwhile()
  if("${SDL2_IMAGE_FRAMEWORK}" MATCHES "(/[^/]+)*.framework$")
    set(SDL2_IMAGE_FRAMEWORK_NAME ${CMAKE_MATCH_1})
    # If we found a framework, do a search for the header ahead of time that
    # will be more likely to get the framework header.
    find_path(
      SDL2_IMAGE_INCLUDE_DIR
      NAMES SDL_image.h # this file was introduced with SDL2
      HINTS "${SDL2_IMAGE_FRAMEWORK}/Headers/")
  else()
    # For some reason we couldn't get the framework directory itself. Shouldn't
    # happen, but might if something is weird.
    unset(SDL2_IMAGE_FRAMEWORK)
  endif()
endif()

find_path(
  SDL2_IMAGE_INCLUDE_DIR
  NAMES SDL_image.h # this file was introduced with SDL2
  HINTS ${SDL2_IMAGE_INCLUDE_HINTS}
  PATHS ${SDL2_IMAGE_ROOT_DIR} ENV SDL2DIR ENV SDL2IMAGEDIR
  PATH_SUFFIXES include include/sdl2 include/SDL2 SDL2)

if(WIN32 AND SDL2_IMAGE_LIBRARY)
  find_file(
    SDL2_IMAGE_RUNTIME_LIBRARY
    NAMES SDL2_image.dll libSDL2_image.dll
    HINTS ${SDL2_IMAGE_LIB_HINTS}
    PATHS ${SDL2_IMAGE_ROOT_DIR} ENV SDL2DIR ENV SDL2IMAGEDIR
    PATH_SUFFIXES bin lib ${SDL2_IMAGE_LIB_PATH_SUFFIX})
  get_filename_component(SDL2_IMAGE_RUNTIME_LIBRARY_FOLDER
                         ${SDL2_IMAGE_RUNTIME_LIBRARY} DIRECTORY)
  file(GLOB SDL2_IMAGE_RUNTIME_LIBRARY_ADDITIONALS
       ${SDL2_IMAGE_RUNTIME_LIBRARY_FOLDER}/*.*)
endif()

if(MINGW AND NOT SDL2PC_FOUND)
  find_library(SDL2_IMAGE_MINGW_LIBRARY mingw32)
  find_library(SDL2_IMAGE_MWINDOWS_LIBRARY mwindows)
endif()

if(SDL2_IMAGE_PREFIX)
  # Restore things the way they used to be.
  set(CMAKE_PREFIX_PATH ${SDL2_IMAGE_ORIGPREFIXPATH})
endif()

# handle the QUIETLY and REQUIRED arguments and set QUATLIB_FOUND to TRUE if all
# listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  SDL2_IMAGE DEFAULT_MSG SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR
  ${SDL2_IMAGE_EXTRA_REQUIRED})

if(SDL2_IMAGE_FOUND)
  if(NOT TARGET SDL2::SDL2_image)
    # Create SDL2::SDL2_image
    if(WIN32 AND SDL2_IMAGE_RUNTIME_LIBRARY)
      set(SDL2_IMAGE_DYNAMIC TRUE)
      add_library(SDL2::SDL2_image SHARED IMPORTED)
      set_target_properties(
        SDL2::SDL2_image
        PROPERTIES IMPORTED_IMPLIB "${SDL2_IMAGE_LIBRARY}"
                   IMPORTED_LOCATION "${SDL2_IMAGE_RUNTIME_LIBRARY}"
                   INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIR}")
    else()
      add_library(SDL2::SDL2_image UNKNOWN IMPORTED)
      if(SDL2_IMAGE_FRAMEWORK AND SDL2_IMAGE_FRAMEWORK_NAME)
        # Handle the case that SDL2 is a framework and we were able to decompose
        # it above.
        set_target_properties(
          SDL2::SDL2_image
          PROPERTIES IMPORTED_LOCATION
                     "${SDL2_IMAGE_FRAMEWORK}/${SDL2_IMAGE_FRAMEWORK_NAME}")
      elseif(_SDL2_IMAGE_framework AND SDL2_IMAGE_LIBRARY MATCHES
                                       "(/[^/]+)*.framework$")
        # Handle the case that SDL2 is a framework and SDL_LIBRARY is just the
        # framework itself.

        # This takes the basename of the framework, without the extension, and
        # sets it (as a child of the framework) as the imported location for the
        # target. This is the library symlink inside of the framework.
        set_target_properties(
          SDL2::SDL2_image PROPERTIES IMPORTED_LOCATION
                                      "${SDL2_IMAGE_LIBRARY}/${CMAKE_MATCH_1}")
      else()
        # Handle non-frameworks (including non-Mac), as well as the case that
        # we're given the library inside of the framework
        set_target_properties(
          SDL2::SDL2_image PROPERTIES IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARY}")
      endif()
      set_target_properties(
        SDL2::SDL2_image PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                    "${SDL2_IMAGE_INCLUDE_DIR}")
    endif()

    if(APPLE)
      # Need Cocoa here, is always a framework
      find_library(SDL2_IMAGE_COCOA_LIBRARY Cocoa)
      list(APPEND SDL2_IMAGE_EXTRA_REQUIRED SDL2_IMAGE_COCOA_LIBRARY)
      if(SDL2_IMAGE_COCOA_LIBRARY)
        set_target_properties(
          SDL2::SDL2_image PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES
                                      ${SDL2_IMAGE_COCOA_LIBRARY})
      endif()
    endif()

    if(MINGW)
      # MinGW requires some additional libraries to appear earlier in the link
      # line.
      if(SDL2PC_LIBRARIES)
        # Use pkgconfig-suggested extra libraries if available.
        list(REMOVE_ITEM SDL2PC_LIBRARIES SDL2main SDL2)
        set(SDL2MAIN_LIBRARIES ${SDL2PC_LIBRARIES} ${SDL2MAIN_LIBRARIES})
      else()
        # fall back to extra libraries specified in pkg-config in an official
        # binary distro of SDL2 for MinGW I downloaded
        if(SDL2_IMAGE_MINGW_LIBRARY)
          set(SDL2MAIN_LIBRARIES ${SDL2_IMAGE_MINGW_LIBRARY}
                                 ${SDL2MAIN_LIBRARIES})
        endif()
        if(SDL2_IMAGE_MWINDOWS_LIBRARY)
          set(SDL2MAIN_LIBRARIES ${SDL2_IMAGE_MWINDOWS_LIBRARY}
                                 ${SDL2MAIN_LIBRARIES})
        endif()
      endif()
    endif()
  endif()
  mark_as_advanced(SDL2_IMAGE_ROOT_DIR)
endif()

mark_as_advanced(
  SDL2_IMAGE_LIBRARY SDL2_IMAGE_RUNTIME_LIBRARY SDL2_IMAGE_INCLUDE_DIR
  SDL2_IMAGE_COCOA_LIBRARY SDL2_IMAGE_MINGW_LIBRARY SDL2_IMAGE_MWINDOWS_LIBRARY)
