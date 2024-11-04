## ---------------------------------------------------------
##  G4_QPIX | src/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------

include_directories(${Geant4_INCLUDE_DIRS})
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

# generate ROOT dictionary
ROOT_GENERATE_DICTIONARY(QPixG4Dict AnalysisManager.h LINKDEF LinkDef.h)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC} QPixG4Dict.cxx)
target_link_libraries(${PROJECT_NAME} stdc++fs)
# link to GEANT4, ROOT, and MARLEY libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} MARLEY)
