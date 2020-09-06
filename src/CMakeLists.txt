## ---------------------------------------------------------
##  G4Basic | src/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------

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
          TrackingHit.cpp)

add_library(${CMAKE_PROJECT_NAME} OBJECT ${SRC})

