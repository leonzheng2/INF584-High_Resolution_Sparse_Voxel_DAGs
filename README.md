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
A ./dag_builder executable is now ready to run.

## Execution Steps
Download a 3D Model in the folder models. It should be in .ply format. Use for example reconstructed models from the [Stanford 3D Scanning library](http://graphics.stanford.edu/data/3Dscanrep/).

### Building a SVO
Build the SVO using the two following executables in the directory linux:
* **tri_convert:** A tool to convert any model file to a simple, streamable .tri format, described in this manual.
* **svo_builder:** Out-Of-Core SVO Builder: Partitioning, voxelizing and SVO Building rolled into one executable, needs a .tri file as input

This should output .octree, .octreenodes, .octreedata files.

### Building the DAG


There are three tools distributed in this release, both are required to convert a model into a Sparse Voxel Octree representation:

* **tri_convert:** A tool to convert any model file to a simple, streamable .tri format, described in this manual.
* **svo_builder:** Out-Of-Core SVO Builder: Partitioning, voxelizing and SVO Building rolled into one executable, needs a .tri file as input
* **dag_builder:** SVO to DAG Builder : Reduces octree to DAG , and writes dag into a file. It can also read dag file and generate voxel points which could be rendered


## Execution Steps
Download any 3D Model in the folder models. The (Stanford 3D Scanning library)[http://graphics.stanford.edu/data/3Dscanrep/] has many to start with. Also there are mutiple free 3D models ar (clara.io)[http://clara.io]

###building executables
 Run the three executables in linux folder
 
* ./build_tri_convert.sh
* ./build_svo_builder.sh
* ./build_dag.sh

This will generate the three binaries as explained above.

Now
**Step 1**: Create .tri file from .ply file 
`./tri_convert -f ../models/dragon.ply`

**Step 2**: Convert tri to octree file
`./svo_builder_binary -f ../models/dragon.tri`

**Step 3**: Create DAG from Octree
`./dag_builder -oct ../models/dragon1024_1.octree`

**Step 4**: Generate point cloud from  DAG
`./dag_builder -odag ../models/dragon1024_1.dag -out ../models/dragon1024_1.pts`

The point cloud file ".pts" contains all the points with range in -1 to 1 in each line. 
To view the point cloud , there is an executable pcloud included. If that executable didn't work. Please build it by following the instructions here
https://www.cse.iitb.ac.in/~cs749/spr2016/a1/a1.html

#DAG File Format
The .dag file contains the following

1. No of nodes
2. Grid lenght
3. The data of each node (Child mask and pointers)

*For details of Other format and executables refer README_old.md*

## Results
See the following doc for experiment results and memory reduction reports
https://docs.google.com/document/d/1Wr1Y1ruIordIbOn6mAJgJbVDMNp5JY9yw-4BJLFlTmc/edit?usp=sharing


## Acknowledgement and References

1. The original Paper : http://www.cse.chalmers.se/~kampe/highResolutionSparseVoxelDAGs.pdf 
2. The SVO Builder code (Base code for this project): https://github.com/Forceflow/ooc_svo_builder
3. Another SVO Builder code (Used for reading): https://github.com/tunabrain/sparse-voxel-octrees  
4. Course material of CS 749 (Used for rendering point clouds) : https://www.cse.iitb.ac.in/~cs749
5. Stanford 3D Scanning Repository : http://graphics.stanford.edu/data/3Dscanrep/
6. Clara.io (Free 3D models) : https://clara.io/library 
7. Google http://www.google.com
