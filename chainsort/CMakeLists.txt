## ---------------------------------------------------------
##  G4_QPIX | app/CMakeLists.txt
##
##  CMake build script.
##   * Author: Kevin Keefe
##   * Creation date: 01 Mar 2023
## ---------------------------------------------------------


add_executable(chainsort chainsorter.cpp)
target_include_directories(chainsort PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(chainsort ${CMAKE_PROJECT_NAME} ${Geant4_LIBRARIES})

## Added for marley
target_link_libraries(chainsort MARLEY)

install(TARGETS G4_QPIX RUNTIME DESTINATION bin)
