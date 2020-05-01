## ---------------------------------------------------------
##  G4Basic | app/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(G4Basic G4Basic.cpp $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}>)
target_include_directories(G4Basic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(G4Basic ${Geant4_LIBRARIES} )



install(TARGETS G4Basic RUNTIME DESTINATION bin)
