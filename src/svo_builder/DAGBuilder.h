//
// Created by leon on 3/9/19.
//

#ifndef HIGH_RESOLUTION_SPARSE_VOXEL_DAGS_DAGBUILDER_H
#define HIGH_RESOLUTION_SPARSE_VOXEL_DAGS_DAGBUILDER_H

#include <string>
#include <vector>
#include "Node.h"
#include "octree_io.h"
#include "DAGNode.h"


class DAGBuilder {
public:
	/*
	 * Constructor
	 */
	DAGBuilder(const string& filename);

    /*
     * Main functions
     */
    void read_from_octree_file(string filename); // Read the .octree and .octreenodes file in memory. Build the octree.
	void reduce(); // Algorithm used in the paper
	void write_dag(); // Write the DAG on the disk

	/*
	 * Secondary functions
	 */
	void printDAG() const; // Showing information about the constructed DAG
	void showLevel(size_t L) const; // Showing all the DAG vertices (children + parents) of level L

private:
	/*
	 * Variables
	 */
	vector<Node> octreeNodes; // Vector of all the SVO nodes read from the input files
	OctreeInfo info; // Information about the SVO
	vector<DAGNode> DAGNodes; // Vector of all DAG vertices. Vertices that have been merged are not deleted, so that the index of the vertex in this vector is the id of the vertex
	vector<vector<size_t>> DAGLevels; // Structure containing the levels of the DAG, and the id of DAG vertices

	/*
	 * DAG functions
	 */
	void constructOriginalDAG(); // Construct the unreduced DAG
	vector<size_t> constructNextDAGLevel(const vector<size_t>& level); // Construct the next DAG level from the previous level
	void fillDAGParents(DAGNode& root); // Fill the parents field of DAGNode
	void sortLevel(size_t L); // Sorting the DAG vertices of the level L
	void groupLevel(size_t L); // Grouping identical DAG vertices (same childmask, same children pointers) and updating parent vertices so that they point to the merged vertices

};

#endif //HIGH_RESOLUTION_SPARSE_VOXEL_DAGS_DAGBUILDER_H
