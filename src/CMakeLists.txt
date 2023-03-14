## ---------------------------------------------------------
##  qpixg4 | src/CMakeLists.txt
##
##  CMake build script.
##   * Author: Q-Pix
##   * Creation date: 1 May 2020
## ---------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/src)

SET(SRC   AnalysisManager.cpp
          GeneratorParticle.cpp
          MARLEYManager.cpp
          MCTruthManager.cpp
          MCParticle.cpp
          DetectorConstruction.cpp
          PrimaryGeneration.cpp
          RunAction.cpp
          EventAction.cpp
          SteppingAction.cpp
          TrackingAction.cpp
          TrackingSD.cpp
          TrackingHit.cpp
          Supernova.cpp
          SupernovaTiming.cpp
          ROOTManager.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC} QPixG4Dict.cxx)

# link to the filesystem library on linux
if (UNIX AND NOT APPLE)
  target_link_libraries(${CMAKE_PROJECT_NAME} stdc++fs)
endif ()

# link to GEANT4, ROOT, and MARLEY libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} MARLEY)

# generate ROOT dictionary
ROOT_GENERATE_DICTIONARY(QPixG4Dict AnalysisManager.h LINKDEF LinkDef.h)

