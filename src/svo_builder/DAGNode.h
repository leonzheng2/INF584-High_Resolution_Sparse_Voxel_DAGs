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

class DAGNode{
public:
    DAGNode(size_t id, Node n); // Constructor from octree node. No parents.
    DAGNode(){}

    size_t key;
    size_t id;
    bool childmask[8];
    std::vector<size_t> children; // Position in the next level
    std::unordered_set<size_t> parents; // Position in the previous level

    bool operator< (const DAGNode &n) const;
    bool operator== (const DAGNode &n) const;
    size_t getChildmask() const;

    bool hasChild(unsigned int i) const;
    size_t getChildPos(unsigned int i) const;

    bool isLeaf() const;

    void print();
    void writeNode(FILE* out);
};

struct DAGNodePtr{
public:
    DAGNode* node;

    DAGNodePtr(DAGNode* n){
        node = n;
    }

    bool operator < (const DAGNodePtr& p) const
    {
        return (*node < *(p.node));
    }
};

#endif //OUTOFCORESPARSEVOXELOCTREE_DAGNODE_H
