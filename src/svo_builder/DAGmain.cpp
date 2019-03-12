// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <ctime>
#include <algorithm>

#include "DAGBuilder.h"

std::string filename;

void printHelp() {
    std::cout << "Example: dag_builder -f /home/models/bunny.octree" << endl;
    std::cout << "" << endl;
    std::cout << "All available program options:" << endl;
    std::cout << "" << endl;
    std::cout << "-f <filename.octree>     Path to a .octree input file." << endl;
    std::cout << "-h                       Print help and exit." << endl;
}

void printInvalid() {
    std::cout << "Not enough or invalid arguments, please try again." << endl;
    std::cout << "At the bare minimum, I need a path to a .octree file" << endl << "" << endl;
    printHelp();
}

void parseProgramParameters(int argc, char* argv[]) {
    cout << "Reading program parameters ..." << endl;
    // Input argument validation
    if (argc < 3) {
        printInvalid();
        exit(0);
    }
    for (int i = 1; i < argc; i++) {
        // parse filename
        if (string(argv[i]) == "-f") {
            filename = argv[i + 1];
            std::cout << filename << std::endl;
            size_t check_tri = filename.find(".octree");
            if (check_tri == string::npos) {
                cout << "Data filename does not end in .octree - I only support that file format" << endl;
                printInvalid();
                exit(0);
            }
            i++;
        }
        else if (string(argv[i]) == "-h") {
            printHelp(); exit(0);
        }
        else {
            printInvalid(); exit(0);
        }
    }
}


int main (int argc, char *argv[]) {

    parseProgramParameters(argc, argv);

    std::clock_t start;
    double duration;

    start = std::clock();
    DAGBuilder dagBuilder(filename.substr(0, filename.size()-7));
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Construct DAG: "<< duration << "s" << '\n';

    start = std::clock();
    dagBuilder.reduce();
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Reduce DAG: "<< duration << "s" << '\n';

    dagBuilder.showLevel(3);
    dagBuilder.printDAG();
    dagBuilder.write_dag();

    return 0;
}