#!/bin/bash

## SPECIFY TRIMESH LOCATION HERE (and do a make there first)
TRIMESH_DIR=../src/libs/trimesh2/
SOURCE_DIR=../src/tri_convert/

## COMPILE AND LINK DEFINITIONS
COMPILE="g++ -c -O3 -m64 -I../src/libtri/include/ -I ${TRIMESH_DIR}/include/"
LINK="g++ -o tri_convert"
LINK_OPTS="-L${TRIMESH_DIR}/lib.Linux64 -ltrimesh -fopenmp -static"

#############################################################################################

## CLEAN
echo "Cleaning old versions ..."
rm -f *.o
rm -f tri_convert
rm -f a.out

## CLEAN
echo "Cleaning..."
rm *.o
rm a.out

## BUILD REGULAR VERSION
echo "Building regular version ..."
${COMPILE} ${SOURCE_DIR}tri_convert.cpp
${LINK} tri_convert.o ${LINK_OPTS}

echo "Done"
