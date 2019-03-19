#!/bin/bash

## SPECIFY TRIMESH LOCATION HERE (and do a make there first)
TRIMESH_DIR=../src/libs/trimesh2/
SOURCE_DIR=../src/svo_builder/
GLM_DIR=../src/libs/glm/

## COMPILE AND LINK DEFINITIONS
COMPILE="g++ -g -c -m64 -O3 -I../src/libs/libtri/include/ -I ${TRIMESH_DIR}/include/ -I ${GLM_DIR}"
LINK="g++ -g -o svo_builder"
LINK_BINARY="g++ -g -o svo_builder_binary"

#############################################################################################
## BUILDING STARTS HERE

## CLEAN
echo "Removing old versions ..."
rm -f svo_builder
rm -f svo_builder_binary
rm -f *.o

## BUILD REGULAR VERSION
echo "Compiling regular version build..."
${COMPILE} ${SOURCE_DIR}main.cpp
${COMPILE} ${SOURCE_DIR}OctreeBuilder.cpp
${COMPILE} ${SOURCE_DIR}partitioner.cpp
${COMPILE} ${SOURCE_DIR}voxelizer.cpp
echo "Linking regular version build..."
${LINK} *.o

echo "Cleaning up ..."
rm *.o

echo "Done"
