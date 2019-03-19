//
// Created by leon on 3/9/19.
//

#include <algorithm>
#include "DAGBuilder.h"

/*
 * Constructor: reading the SVO and build the unreduced DAG
 */

DAGBuilder::DAGBuilder(const string& filename) {
    read_from_octree_file(filename);
    constructOriginalDAG();
}

void DAGBuilder::read_from_octree_file(string filename){

    // Parse Octree info
    parseOctreeHeader(filename + ".octree", info);
    if(info.filesExist()){
        info.print();
    } else {
        std::cerr << "*.octree files don't exist. Please check that ./svo_builder have constructed those files." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Reading nodes information
    FILE * file = fopen(string(filename + ".octreenodes").c_str(), "r");
    if(file == NULL){
        perror("Error opening file");
    } else {
        std::cout << "Reading nodes..." << std::endl;
        int i = 0;
        while(!feof(file)){
            Node n;
            readNode(file, n);
            if(i%100000 == 0)
                std::cout << i << "/" << info.n_nodes << std::endl;
            octreeNodes.push_back(n);
            i++;
        }
        std::cout << "All nodes are in memory" << std::endl;
    }
}

void DAGBuilder::constructOriginalDAG() {
    for(size_t i=0; i<octreeNodes.size(); i++){
        DAGNode DAGnode(i, octreeNodes[i]);
        DAGNodes.push_back(DAGnode);
    }
    // Fill parents field
    DAGNode root = DAGNodes[DAGNodes.size()-2]; // Root is at this position by construction of the file .octreenode
    fillDAGParents(root);

    // Construct level for DAG reduction
    DAGLevels.push_back({DAGNodes.size()-2});
    unsigned long L = DAGLevels.size();
    vector<size_t> nextLevel = constructNextDAGLevel(DAGLevels[L-1]);
    while(!nextLevel.empty()){
        DAGLevels.push_back(nextLevel);
        L++;
        nextLevel = constructNextDAGLevel(DAGLevels[L-1]);
    }
}

void DAGBuilder::fillDAGParents(DAGNode &root) {
    // Recursive function
    for(size_t child: root.children){
        DAGNodes[child].parents.insert(root.id);
        fillDAGParents(DAGNodes[child]);
    }
}

vector<size_t> DAGBuilder::constructNextDAGLevel(const vector<size_t> &level) {
    vector<size_t> nextLevel; // is empty when we are at leaf level
    for(const size_t& n_id: level){
        for(size_t child: DAGNodes[n_id].children){
            nextLevel.push_back(child);
        }
    }
    return nextLevel;
}

/*
 * Reducing an SVO to a DAG
 */

void DAGBuilder::reduce() {
    for(size_t L=DAGLevels.size()-1; L>=0 && L<DAGLevels.size(); L=L-1){
        sortLevel(L);
        groupLevel(L);
    }
}


void DAGBuilder::sortLevel(size_t L) {
    // Construct vector of pointer so that we can apply sorting on it
    vector<DAGNodePtr> level;
    for(size_t n_id: DAGLevels[L]){
        level.push_back(DAGNodePtr(&(DAGNodes[n_id])));
    }
    // Sort pointers is faster than sorting objects
    std::sort(level.begin(), level.end());
    // Writing the sorted vector
    for(int i=0; i<level.size(); i++){
        DAGLevels[L][i] = (level[i].node)->id;
    }
}

void DAGBuilder::groupLevel(size_t L) {
    // Initialize new level
    vector<size_t> newLevel = {DAGLevels[L][0]};
    // Id of the node in this new list for comparison with old list
    size_t group_id = newLevel[newLevel.size()-1];

    // Assert that the level is sorted
    for(int i=1; i<DAGLevels[L].size(); i++){ // Old list
        size_t n_id = DAGLevels[L][i];
        if(DAGNodes[n_id] == DAGNodes[group_id]){ // Regrouping the nodes
            for(size_t parent: DAGNodes[n_id].parents){
                // Update the parents field of the regrouped node
                DAGNodes[group_id].parents.insert(parent);
                // Updating the children field of the parent node
                for(size_t& child: DAGNodes[parent].children){
                    if(child == n_id)
                        child = group_id;
                }
            }
        } else { // New group of nodes
            newLevel.push_back(n_id);
        }
    }

    // Change the old level by the new level
    DAGLevels[L] = newLevel;
}

/*
 * Secondary methods for printing and output
 */

void DAGBuilder::write_dag() {
    FILE* file = fopen(string(info.base_filename + ".dag").c_str(), "wb");
    for(size_t L=0; L<DAGLevels.size(); L++){
        for(size_t i=0; i<DAGLevels[L].size(); i++){
            size_t n_id = DAGLevels[L][i];
            DAGNodes[n_id].writeNode(file);
        }
    }
    fclose(file);
}

void DAGBuilder::printDAG() const{
    int n = 0;
    for(int L=0; L<DAGLevels.size(); L++){
        n += DAGLevels[L].size();
    }
    std::cout << "Checking DAGLevels..." << std::endl;
    std::cout << "There are " << n << " DAG nodes, against " << info.n_nodes << " in SVO." << std::endl;
}

void DAGBuilder::showLevel(size_t L) const {
    std::cout << "Level " << L << std::endl;
    for(size_t n_id: DAGLevels[L]){
        DAGNode n = DAGNodes[n_id];
        n.print();
    }
    std::cout << "There are " << DAGLevels[L].size() << " nodes at level " << L << std::endl;
}


