# INF584 - High Resolution Sparse Voxel DAGs
This is an implementation of the reducing an SVO into a Sparse Voxel DAG algorithm from [High Resolution Sparse Voxel DAGs](http://www.cse.chalmers.se/~uffe/HighResolutionSparseVoxelDAGs.pdf).

The base code for this implementation is from [Out-of-Core Construction of Sparse Voxel Octrees](https://github.com/Forceflow/ooc_svo_builder).

## Building / Dependencies
All the libraries needed are those needed to run the implementation of the Sparse Voxel Octree builder. Please refer to the [Github page](https://github.com/Forceflow/ooc_svo_builder) of this implementation for more details.

The libraries needed are included in src/libs, and are already set up in the Git repository:
* **libmorton**
* **libtri**
* **trimesh2**
* **glm**

Once libraries are ready, build the project:
```
mkdir build
cd build/
cmake ..
make
```
A **dag_builder** executable is now ready to run.

To rebuild the **tri_convert** and **svo_builder** executables in the linux folder, which are needed for the construction of the SVO, one can run in the linux folder:
* ./build_tri_convert.sh
* ./build_svo_builder.sh

## Execution Steps
Download a 3D Model in the folder models. It should be in .ply format. Use for example reconstructed models from the [Stanford 3D Scanning library](http://graphics.stanford.edu/data/3Dscanrep/).

### Building a SVO
Build the SVO using the two following executables in the directory linux:
* **tri_convert:** A tool to convert any model file to a simple, streamable .tri format.
* **svo_builder:** Out-Of-Core SVO Builder: Partitioning, voxelizing and SVO Building rolled into one executable, needs a .tri file as input

**Step 1**: Create .tri file from .ply file 
`./tri_convert -f ../models/dragon.ply`

**Step 2**: Convert tri to octree file
`./svo_builder_binary -f ../models/dragon.tri`

This should output .octree, .octreenodes, .octreedata files.

### Building the DAG
Reduce the SVO by using the executable in build directory:
* **dag_builder:** My implementation of the DAG reducing algorithm, needs the previous .octree, .octreenodes, .octreedata files.

**Step 3**: Create DAG from Octree
`./dag_builder -oct ../models/dragon1024_1.octree`

This should ouput a .dag file containing the Sparse Voxel DAG of the scene, encoded as the following way:
1. The childmask of the mode
2. The children pointers, in the order of the childmask

## Source code
The code used for creating the **dag_builder** executable is in the folder src/svo_builder and there are contained in the files beginning with DAG:
* DAGmain.cpp
* DAGBuilder.h
* DAGBuilder.cpp
* DAGNode.cpp
* DAGNode.cpp
