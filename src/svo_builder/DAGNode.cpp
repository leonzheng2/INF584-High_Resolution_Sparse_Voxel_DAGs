//
// Created by leon on 3/10/19.
//

#include <iostream>
#include "DAGNode.h"

/*
 * Constructor
 */
DAGNode::DAGNode(size_t id, Node n) {
    this->id = id;
    childmask = 0;
    for(unsigned j=0; j<8; j++){
        childmask = (childmask << 1) + n.hasChild(j);
        if(n.hasChild(j)){
            children.push_back(n.getChildPos(j));
        }
    }
}

/*
 * Overloading '<' and '=='
 */
bool DAGNode::operator< (const DAGNode &n) const {
    if(childmask!=n.childmask)
        return childmask < n.childmask;
    for(int j=0; j<children.size(); j++){
        if(children[j]!=n.children[j])
            return children[j] < n.children[j];
    }
    return false;
}

bool DAGNode::operator==(const DAGNode &n) const {
    if(childmask!=n.childmask)
        return false;
    for(int j=0; j<children.size(); j++){
        if(children[j]!=n.children[j])
            return false;
    }
    return true;
}

bool DAGNode::isLeaf() const {
    return children.empty();
}

void DAGNode::print(){
    string s = "DAGNode " + std::to_string(id) + " - childmask: " + std::to_string(childmask) + " (";
    int a = childmask;
    for(int j=7; j>=0; j--){
        s+= a % int(pow(2, j)) == a ? "0" : "1";
        a = a % int(pow(2, j));
    }
    s += "); ";
    s += "children: ";
    for(size_t child: children){
        s += std::to_string(child) + "; ";
    }
    s += "parents: ";
    for(size_t parent: parents){
        s += std::to_string(parent) + "; ";
    }
    std::cout << s << std::endl;
}

void DAGNode::writeNode(FILE *out) {
    fwrite(&childmask, sizeof(uint8_t), 1, out);
    if(!isLeaf())
        fwrite(&children[0], sizeof(size_t), children.size(), out);
}
