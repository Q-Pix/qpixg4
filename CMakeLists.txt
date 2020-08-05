## ---------------------------------------------------------
##  G4Basic | CMakeLists.txt
##
##  Main CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------
#set( ENV{PATH} $ENV{NOCONDA_PATH} )
#MESSAGE("PATH VAR $ENV{PATH}")

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(G4BASIC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Find Geant4 package, activating all available UI and Vis
## drivers by default. You can set WITH_GEANT4_UIVIS to OFF
## via the command line or ccmake/cmake-gui to build a batch
## mode only executable.
option(WITH_GEANT4_UIVIS "Build with Geant4 UI/VIS drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

## Setup Geant4 include directories and compile definitions.
include(${Geant4_USE_FILE})

## find the ROOT package
find_package(ROOT REQUIRED)

## include ROOT header files
include(${ROOT_USE_FILE})

## link ROOT libraries
link_libraries(${ROOT_LIBRARIES})

## Recurse through sub-directories
add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(cfg)
