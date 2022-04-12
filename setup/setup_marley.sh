#!/bin/sh

################################################################
# setup_marley.sh
#
#  Author: Dave Elofson
#  Created: 2022-04-05
################################################################

# Intended use:
# 
# source setup_marley.sh [opt. path to marley directory if not default from ReleaseBuilder]
#
# Only needs to be run before building the first time. Runs setup_marley.sh in marley repository, then sets CPLUS_INCLUDE_PATH and
# LIBRARY_PATH for building against Marley.
#
# NOTE: This uses the assumption that marley is built in the same directory as qpixg4, as ReleaseBuilder would do. If that is not
# the case, you will need to change 


initdir=$(pwd -P)
echo The initial directory is $initdir 
marleypath=${1:-"$initdir/../marley"}
echo The marleypath is $marleypath

echo changing to $marleypath 
cd $marleypath
echo the new directory is $(pwd -P)

echo sourcing setup_marley.sh
source ./setup_marley.sh


echo Making it possible to build against marley...
export CPLUS_INCLUDE_PATH=${CPLUS_INCLUDE_PATH}:$/include
export LIBRARY_PATH=${LIBRARY_PATH}:${THIS_DIRECTORY}/build


echo changing back to $initdir
cd $initdir


