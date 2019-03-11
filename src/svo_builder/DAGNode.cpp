//
// Created by leon on 3/10/19.
//

#include <iostream>
#include "DAGNode.h"


DAGNode::DAGNode(size_t id, Node n) {
    this->id = id;
    for(unsigned j=0; j<8; j++){
        childmask[j] = n.hasChild(j);
        if(childmask[j]){
            children.push_back(n.getChildPos(j));
        }
    }
    key = getChildmask();
}

bool DAGNode::operator< (const DAGNode &n) const {
    if(key!=n.key)
        return key < n.key;
    for(int j=0; j<children.size(); j++){
        if(children[j]!=n.children[j])
            return children[j] < n.children[j];
    }
    return false;
}

bool DAGNode::operator==(const DAGNode &n) const {
    if(getChildmask()!=n.getChildmask())
        return false;
    for(int j=0; j<children.size(); j++){
        if(children[j]!=n.children[j])
            return false;
    }
    return true;
}

size_t DAGNode::getChildmask() const {
    int s=0;
    for(int j=0; j<8; j++){
        s = (s << 1) + childmask[j];
    }
    return s;
}

bool DAGNode::hasChild(unsigned int i) const {
    return childmask[i];
}

size_t DAGNode::getChildPos(unsigned int i) const {
    for(int j=0; j<8; j++){
        if(childmask[j])
            i--;
        if(i<0)
            return children[j];
    }
}

bool DAGNode::isLeaf() const {
    return children.empty();
}

void DAGNode::print(){
    string s = "DAGNode " + std::to_string(id) + " - childmask: " + std::to_string(getChildmask()) + " (";
    for(int j=0; j<8; j++){
        s += std::to_string(childmask[j]);
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
    fwrite(&childmask[0], sizeof(bool), 8, out);
    if(!isLeaf())
        fwrite(&children[0], sizeof(size_t), children.size(), out);
}
