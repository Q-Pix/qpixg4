## ---------------------------------------------------------
##  G4Basic | app/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------


add_executable(G4Basic G4Basic.cpp $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}>)
target_include_directories(G4Basic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(G4Basic ${Geant4_LIBRARIES} )

## Added for marley
target_link_libraries(G4Basic MARLEY)


## some debuging fustration...
#message(STATUS "AUSTIN------------------------------------------{")
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "includes ='${dir}'")
#endforeach()
#message(STATUS "AUSTIN------------------------------------------}")

#message(STATUS "AUSTIN------------------------------------------{")
#get_target_property(dirs G4Basic LINK_LIBRARIES)
#foreach(dir ${dirs})
#  message(STATUS "Libarys='${dir}'")
#endforeach()
#message(STATUS "AUSTIN------------------------------------------}")


install(TARGETS G4Basic RUNTIME DESTINATION bin)
