# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build

# Include any dependencies generated for this target.
include app/CMakeFiles/G4Basic.dir/depend.make

# Include the progress variables for this target.
include app/CMakeFiles/G4Basic.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/G4Basic.dir/flags.make

app/CMakeFiles/G4Basic.dir/G4Basic.cpp.o: app/CMakeFiles/G4Basic.dir/flags.make
app/CMakeFiles/G4Basic.dir/G4Basic.cpp.o: ../app/G4Basic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/CMakeFiles/G4Basic.dir/G4Basic.cpp.o"
	cd /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/G4Basic.dir/G4Basic.cpp.o -c /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/app/G4Basic.cpp

app/CMakeFiles/G4Basic.dir/G4Basic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/G4Basic.dir/G4Basic.cpp.i"
	cd /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/app/G4Basic.cpp > CMakeFiles/G4Basic.dir/G4Basic.cpp.i

app/CMakeFiles/G4Basic.dir/G4Basic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/G4Basic.dir/G4Basic.cpp.s"
	cd /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/app/G4Basic.cpp -o CMakeFiles/G4Basic.dir/G4Basic.cpp.s

# Object files for target G4Basic
G4Basic_OBJECTS = \
"CMakeFiles/G4Basic.dir/G4Basic.cpp.o"

# External object files for target G4Basic
G4Basic_EXTERNAL_OBJECTS = \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/AnalysisManager.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/GeneratorParticle.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/MARLEYManager.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/MCTruthManager.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/MCParticle.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/DetectorConstruction.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/PrimaryGeneration.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/RunAction.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/EventAction.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/SteppingAction.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/TrackingAction.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/TrackingSD.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/TrackingHit.cpp.o" \
"/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/src/CMakeFiles/G4BASIC.dir/Supernova.cpp.o"

app/G4Basic: app/CMakeFiles/G4Basic.dir/G4Basic.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/AnalysisManager.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/GeneratorParticle.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/MARLEYManager.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/MCTruthManager.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/MCParticle.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/DetectorConstruction.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/PrimaryGeneration.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/RunAction.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/EventAction.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/SteppingAction.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/TrackingAction.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/TrackingSD.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/TrackingHit.cpp.o
app/G4Basic: src/CMakeFiles/G4BASIC.dir/Supernova.cpp.o
app/G4Basic: app/CMakeFiles/G4Basic.dir/build.make
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libCore.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libImt.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libRIO.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libNet.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libHist.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libGraf.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libGraf3d.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libGpad.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libROOTDataFrame.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libTree.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libTreePlayer.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libRint.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libPostscript.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libMatrix.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libPhysics.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libMathCore.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libThread.so
app/G4Basic: /Users/austinmcdonald/software/ROOT/build/lib/libMultiProc.so
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4Tree.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4GMocren.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4visHepRep.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4RayTracer.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4VRML.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4OpenGL.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4gl2ps.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4interfaces.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4persistency.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4error_propagation.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4readout.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4physicslists.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4parmodels.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4FR.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4vis_management.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4modeling.dylib
app/G4Basic: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/OpenGL.framework/OpenGL.tbd
app/G4Basic: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/OpenGL.framework/OpenGL.tbd
app/G4Basic: /usr/X11R6/lib/libXmu.dylib
app/G4Basic: /usr/X11R6/lib/libXext.dylib
app/G4Basic: /usr/X11R6/lib/libXt.dylib
app/G4Basic: /usr/X11R6/lib/libSM.dylib
app/G4Basic: /usr/X11R6/lib/libICE.dylib
app/G4Basic: /usr/X11R6/lib/libX11.dylib
app/G4Basic: /usr/X11R6/lib/libGLU.dylib
app/G4Basic: /usr/X11R6/lib/libGL.dylib
app/G4Basic: /usr/local/Cellar/xerces-c/3.2.2/lib/libxerces-c.so
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4run.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4event.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4tracking.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4processes.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4analysis.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4zlib.dylib
app/G4Basic: /usr/lib/libexpat.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4digits_hits.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4track.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4particles.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4geometry.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4materials.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4graphics_reps.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4intercoms.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4global.dylib
app/G4Basic: /Users/austinmcdonald/software/GEANT4/geant4.10.06/lib/libG4clhep.dylib
app/G4Basic: app/CMakeFiles/G4Basic.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable G4Basic"
	cd /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/G4Basic.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/CMakeFiles/G4Basic.dir/build: app/G4Basic

.PHONY : app/CMakeFiles/G4Basic.dir/build

app/CMakeFiles/G4Basic.dir/clean:
	cd /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app && $(CMAKE_COMMAND) -P CMakeFiles/G4Basic.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/G4Basic.dir/clean

app/CMakeFiles/G4Basic.dir/depend:
	cd /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4 /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/app /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app /Users/austinmcdonald/projects/tmp/Q_PIX_GEANT4/Build/app/CMakeFiles/G4Basic.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/G4Basic.dir/depend
