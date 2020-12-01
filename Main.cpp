// Main file of the FourConnect Project
// Performs an Alpha-Beta Tree Search to find the next best move in the Game.
#include "GameState.h"
#include "Globals.h"
#include "Solver.h"

#include <iostream>
#include <chrono>

using namespace std;


void printUsage() {
    cout << "Usage: FourConnect <State in XO- format> [-depth=<int>] [-maxtime=<int>] [-v] \n";
    cout << "Perform an alpha-beta pruning tree search to solve the connect four game.\n";
    cout << "-depth: Maximum search depth in the tree (default: 5)\n";
    cout << "-maxtime: Approximate maximum time for the search. This time is not a hard limit, in most cases the runtime will stay below this limit. \n";
}

bool readArg(std::string val, solver::ConnectFourArgs &args) {
    if (val.length() > 7 && val.substr(0, 7) == "-depth=") {
        int depth = std::atoi(val.substr(7, val.size()).c_str());
        if (depth <= 0 || depth > 100) {
            cout << "Illegal depth of " << depth << ".\n";
            return false;
        }
        else {
            args.maxdepth = depth;
        }
    } else if (val.length() > 9 && val.substr(0, 9) == "-maxtime=") {
        int maxtime = std::atoi(val.substr(9, val.size()).c_str());
        if (maxtime <= 0) {
            cout << "Illegal time value of " << maxtime << ".\n";
            return false;
        }
        else {
            args.maxtime = maxtime;
        }
    }
    else if (val == "-v" || val == "-verbose") {
        args.verbose = true;
    }
    else{
        return false;
    }
    return true;
  
}



int main(int argc, char *argv[]) {
    // Usage: FourConnect <State in XO- format> [-depth=<int>] [-v]
    // Parse the specified input file.
    if (argc == 1) {
        printUsage();
        cout << "Provide at least one argument.\n";
        return 0;
    }

    GameState state;
    if (argc > 1) {
        if (!parseState(argv[1], state)) {
            cout << "Error parsing initial state.\n";
            return -1;
        }

    }
    solver::ConnectFourArgs myArgs;
    for (int i = 2; i < argc; i++) {
        if (!readArg(argv[i], myArgs)) {
            return -1;
        }
    }
    if (!state.isValidState()) {
        cout << "Error: Illegal initial state.\n";
        return -1;
    }
    int voidval = 0;
    int a = solver::interative_deepening(state, myArgs, voidval);
    cout << a << std::endl;
}



