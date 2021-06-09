## ---------------------------------------------------------
##  G4_QPIX | app/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------


add_executable(G4_QPIX G4_QPIX.cpp)
target_include_directories(G4_QPIX PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(G4_QPIX ${CMAKE_PROJECT_NAME} ${Geant4_LIBRARIES})

## Added for marley
target_link_libraries(G4_QPIX MARLEY)

install(TARGETS G4_QPIX RUNTIME DESTINATION bin)
