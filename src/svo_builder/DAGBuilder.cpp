//
// Created by leon on 3/9/19.
//

#include <algorithm>
#include <ctime>
#include "DAGBuilder.h"


DAGBuilder::DAGBuilder(const string& filename) {
    read_from_octree_file(filename);
    constructOriginalDAG();
}

void DAGBuilder::read_from_octree_file(string filename){

    // Parse Octree info
    if(info.filesExist()){
        parseOctreeHeader(filename, info);
        info.print();
    } else {
        std::cerr << "*.octree files don't exist. Please check that ./svo_builder have constructed those files." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Reading nodes information
    FILE * file = fopen(string(filename + "nodes").c_str(), "r");
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

    // Constructing levels of nodes
    constructOctreeLevels();
    std::cout << "There are " << octreeLevels.size() << " level(s)" << std::endl;
    // Checking the number of nodes in octreeLevels
    size_t n = 0;
    for(int L=0; L<octreeLevels.size(); L++){
        n += octreeLevels[L].size();
    }
    std::cout << "There are " << n << " nodes in octreeLevels, and information shows " << info.n_nodes << " nodes." << std::endl;
}

void DAGBuilder::reduce() {
    for(size_t L=DAGLevels.size()-1; L>=0 && L<DAGLevels.size(); L=L-1){
        std::clock_t start;
        double duration;

        start = std::clock();
        sortLevel(L);
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout<<"Sorting level " << L << " : "<< duration <<'\n';

        start = std::clock();
        groupLevel(L);
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout<<"Grouping level " << L << " : "<< duration <<'\n';

//        showLevel(L);
    }
}

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

void DAGBuilder::checkRootNode() {
    // Let's check if the last node is the root node
    unsigned long root_i = 1348284;
    Node root = octreeNodes[root_i];
    bool check = true;
    for(int i=0; i<octreeNodes.size(); i++){
        Node n = octreeNodes[i];
        for(unsigned int j=0; j<8; j++){
            if(n.hasChild(j) && root_i == n.getChildPos(j)){
                std::cout << "Found the root node " << root_i << " as a children node of " << i << std::endl;
                check = false;
            }
        }

    }
    if(check){
        std::cout << "The node " << root_i << " is the root" << std::endl;
    } else {
        std::cout << "The node " << root_i << " isn't the root" << std::endl;
    }
}

void DAGBuilder::printLeaves() {
    int count = 0;
    for(int i=0; i<octreeNodes.size(); i++){
        Node n = octreeNodes[i];
        if(n.isLeaf()){
            std::cout << "Leaf at node " << i << ": ";
            for(int j=0; j<8; j++){
                std::cout << n.hasChild(j);
            }
            std::cout << std::endl;
            count++;
        }
    }
    std::cout << "There are " << count << " leaves" << std::endl;
}

void DAGBuilder::constructOctreeLevels() {
    Node root = octreeNodes[octreeNodes.size()-2];
    // Octree root level
    octreeLevels.push_back({root}); // Add the root level
    // Initialize level L
    unsigned long L = octreeLevels.size(); // L = 1;

    vector<Node> nextLevel = constructNextOctreeLevel(octreeLevels[L-1]);
    while(!nextLevel.empty()){
        octreeLevels.push_back(nextLevel);

        L++;
        nextLevel = constructNextOctreeLevel(octreeLevels[L-1]);
    }

}

vector<Node> DAGBuilder::constructNextOctreeLevel(const vector<Node>& level) {
    vector<Node> nextLevel;
    for(Node n: level){
        for(unsigned int j=0; j<8; j++){
            if(n.hasChild(j))
                nextLevel.push_back(octreeNodes[n.getChildPos(j)]);
        }
    }
    return nextLevel;
}

void DAGBuilder::printOctree() {
    for(int i=0; i<octreeNodes.size(); i++){
        std::cout << "Node " << i << ": ";
        for(unsigned int j=0; j<8; j++){
            if(octreeNodes[i].hasChild(j)){
                std::cout << octreeNodes[i].getChildPos(j) << "; ";
            } else {
                std::cout << "-1; ";
            }
        }
        std::cout << std::endl;
    }
}

void DAGBuilder::constructOriginalDAG() {
    for(size_t i=0; i<octreeNodes.size(); i++){
        DAGNode DAGnode(i, octreeNodes[i]);
        DAGNodes.push_back(DAGnode);
    }
    // Add parents
    DAGNode root = DAGNodes[DAGNodes.size()-2];
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

vector<size_t> DAGBuilder::constructNextDAGLevel(const vector<size_t> &level) {
    vector<size_t> nextLevel;
    for(const size_t& n_id: level){
        for(size_t child: DAGNodes[n_id].children){
            nextLevel.push_back(child);
        }
    }
    return nextLevel;
}

void DAGBuilder::fillDAGParents(DAGNode &root) {
    for(size_t child: root.children){
        DAGNodes[child].parents.insert(root.id);
        fillDAGParents(DAGNodes[child]);
    }
}

void DAGBuilder::printDAG() const{
    int n = 0;
    for(int L=0; L<DAGLevels.size(); L++){
        n += DAGLevels[L].size();
    }
    std::cout << "Checking DAGLevels..." << std::endl;
    std::cout << "There are " << n << " DAG nodes, against " << info.n_nodes << " in octree." << std::endl;
}

void DAGBuilder::sortLevel(size_t L) {
    vector<DAGNodePtr> level;
    for(size_t n_id: DAGLevels[L]){
        level.push_back(DAGNodePtr(&(DAGNodes[n_id])));
    }

    std::clock_t start;
    double duration;

    start = std::clock();
    std::sort(level.begin(), level.end());
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Calling std::sort at level " << L << " : "<< duration <<'\n';

    for(int i=0; i<level.size(); i++){
        DAGLevels[L][i] = (level[i].node)->id;
    }
}

void DAGBuilder::showLevel(size_t L) const {
    std::cout << "Level " << L << std::endl;
    for(size_t n_id: DAGLevels[L]){
        DAGNode n = DAGNodes[n_id];
        n.print();
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

