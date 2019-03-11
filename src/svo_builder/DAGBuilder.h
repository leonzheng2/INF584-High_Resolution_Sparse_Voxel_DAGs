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
	 * Variables
	 */
    vector<Node> octreeNodes;
    vector<vector<Node>> octreeLevels;
	OctreeInfo info;

	vector<DAGNode> DAGNodes;
    vector<vector<size_t>> DAGLevels;

    /*
     * Main functions
     */
    void read_from_octree_file(string filename); // Read the .octree and .octreenodes file in memory. Build the octree.
	void reduce(); // Algorithm used in the paper
	void write_dag(); // Write the DAG on the disk


	/*
	 * Octree functions
	 */
	void constructOctreeLevels();
    vector<Node> constructNextOctreeLevel(const vector<Node>& level);
	void checkRootNode();
	void printLeaves();
	void printOctree();

	/*
	 * DAG functions
	 */
	void constructOriginalDAG();
	vector<size_t> constructNextDAGLevel(const vector<size_t>& level);
	void fillDAGParents(DAGNode& root);
	void printDAG() const;
	void sortLevel(size_t L);
	void showLevel(size_t L) const;
	void groupLevel(size_t L);

};

#endif //HIGH_RESOLUTION_SPARSE_VOXEL_DAGS_DAGBUILDER_H
