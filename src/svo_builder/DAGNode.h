//
// Created by leon on 3/10/19.
//

#ifndef OUTOFCORESPARSEVOXELOCTREE_DAGNODE_H
#define OUTOFCORESPARSEVOXELOCTREE_DAGNODE_H

#include <stdlib.h>
#include <cmath>
#include <vector>
#include <ostream>
#include <unordered_set>
#include "Node.h"

/*
 * DAG vertice data structure, containing childmask, children pointers, but also parents pointers, id for easier DAG construction.
 */
class DAGNode{
public:
    // Constructors
    DAGNode(size_t id, Node n); // Constructor from octree node. No parents.
    DAGNode(){}

    // Attributes
    std::vector<size_t> children; // Position in the next level
    std::unordered_set<size_t> parents; // Position in the previous level
    size_t id; // Vertex index in DAGNodes vector

    // Overloading
    bool operator< (const DAGNode &n) const; // Compare childmask. If identical, use lexicographic order on children
    bool operator== (const DAGNode &n) const; // Same childmask, same children

    // Secondary methods
    bool isLeaf() const; // Check is there are children
    void print(); // Print DAG vertex with id, childmask, children and parents
    void writeNode(FILE* out); // Write one node in file out

private:
    uint8_t childmask; // 8bits, '1' if there is a child
};

/*
 * Structure containing a node pointer, with an overloaded < operator so that we can sort on those structures
 */
struct DAGNodePtr{
public:
    DAGNode* node;
    DAGNodePtr(DAGNode* n){ node = n; }
    bool operator < (const DAGNodePtr& p) const { return (*node < *(p.node)); }
};

#endif //OUTOFCORESPARSEVOXELOCTREE_DAGNODE_H
