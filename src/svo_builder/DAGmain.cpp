// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <ctime>
#include <algorithm>

#include "DAGBuilder.h"

void parseProgramParameters(int argc, char* argv[]) {
    // TODO
}


int main (int argc, char *argv[]) {

    parseProgramParameters(argc, argv);

    std::string filename("../models/dragon1024_1");

    DAGBuilder dagBuilder(filename);

    std::clock_t start;
    double duration;

    start = std::clock();
    dagBuilder.constructOriginalDAG();
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Construct DAG: "<< duration <<'\n';

    start = std::clock();
    dagBuilder.reduce();
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Reduce DAG: "<< duration <<'\n';

    dagBuilder.printDAG();
    dagBuilder.write_dag();

    return 0;
}