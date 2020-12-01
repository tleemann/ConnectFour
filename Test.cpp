#include "GameState.h"
#include "Globals.h"
#include "Solver.h"
#include "MoveCache.h"

#include <string>
#include <iostream>
template<typename A>
bool compareOutputs(const A& res, const A& exp, std::string testname="") {
    if (res != exp) {
        if (testname == "") {
            std::cout << "Comparison failed: Expected " << exp << " got " << res << "." << std::endl;
        }
        else {
            std::cout << "Test " << testname << " failed: Expected " << exp << " got " << res << "." << std::endl;
        }
        return false;
    }
    return true;
}


// Test the move cache
bool testCache() {
    GameState state;
    parseState("...O....", state);
    MoveCache myCache;
    
    bool success = true;
    success = compareOutputs(myCache.isHit(state, 0), false, "Hit not added.") & success;

    // Add to cache.
    myCache.addState(state, true, 3, 5);
    success = compareOutputs(myCache.isHit(state, 6), true, "Hit after add.") & success;
    success = compareOutputs(myCache.getMinMaxVal(state), 5, "Value after add.") & success;

    // Check size and hit cound
    success = compareOutputs((int)myCache.getCacheSize(), 1, "Cache size.") & success;
    success = compareOutputs(myCache.getHits(), 1, "Count hits.") & success;

    return success;
}

bool testState() {
    GameState state;
    bool success = true;
    parseState("O.XOXX.O.OXXOXX.XOXOX.O.OOO.", state);
    success = compareOutputs(state.get(1, 1), -1, "Get 1,1") & success;
    success = compareOutputs(state.get(3, 1), 1, "Get 3,1") & success;
    success = compareOutputs(state.get(3, 5), 0, "Get 3,5") & success;
    
    state.set(1, 1, 1);
    success = compareOutputs(state.get(1, 1), 1, "Get 1,1 after set") & success;

    state.set(3, 1, 0);
    success = compareOutputs(state.get(3, 1), 0, "Get 3,1 after set") & success;

    return success;
}

// Test some moves
bool testGameDepthSeven() {
    bool success = true;
    GameState state;
    int mmval;
    int action;
    globvars::verbose = true;

    solver::ConnectFourArgs myArgs;
    myArgs.maxdepth = 7;
    myArgs.maxtime = 20000;

    parseState("...O....", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 3, "Action 0") & success;
    success = compareOutputs(mmval, 0, "Value 0") & success;

    // Not part of original game but good tests as well.
    parseState("...OX.O...", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 2, "Action 1a") & success;
    success = compareOutputs(mmval, 1, "Value 1a") & success;

    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 2, "Action 1a IT") & success;
    success = compareOutputs(mmval, 1, "Value 1a IT") & success;

    parseState("..XO.OXXO.O...", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 5, "Action 3a") & success;
    success = compareOutputs(mmval, 2, "Value 3a") & success;
    
    // Back to the game
    parseState("..O.OX....", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 4, "Action 1") & success;
    success = compareOutputs(mmval, 1, "Value 1") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 4, "Action 1") & success;
    success = compareOutputs(mmval, 1, "Value 1") & success;

    parseState("..O.OX.XO...", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 3, "Action 2") & success;
    success = compareOutputs(mmval, 2, "Value 2") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 3, "Action 2") & success;
    success = compareOutputs(mmval, 2, "Value 2") & success;

    parseState("..O.OXXO.XO...", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 1, "Action 3") & success;
    success = compareOutputs(mmval, 2, "Value 3") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 1, "Action 3 It") & success;
    success = compareOutputs(mmval, 2, "Value 3 It") & success;

    parseState(".XO.O.OXXO.XO...", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 3, "Action 4 IT") & success;
    success = compareOutputs(mmval, 1, "Value 4 IT") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 3, "Action 4 IT") & success;
    success = compareOutputs(mmval, 1, "Value 4 IT") & success;

    parseState("O.XO.O.OXXOX.XO...", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 1, "Action 5") & success;
    success = compareOutputs(mmval, 1, "Value 5") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 1, "Action 5 IT") & success;
    success = compareOutputs(mmval, 1, "Value 5 IT") & success;

    parseState("O.XOX.O.OXXOX.XO..O.", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 1, "Action 6") & success;
    success = compareOutputs(mmval, 3, "Value 6") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 1, "Action 6 IT") & success;
    success = compareOutputs(mmval, 3, "Value 6 IT") & success;

    parseState("O.XOXX.O.OXXOX.XO.O.O.", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 4, "Action 7") & success;
    success = compareOutputs(mmval, 4, "Value 7") & success;
    action = solver::interative_deepening(state, myArgs, mmval);

    // With depth=25, its possible to find out that we lost here.
    parseState("O.XOXX.O.OXXOX.XOXO.O.O.", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 4, "Action 8") & success;
    success = compareOutputs(mmval, 4, "Value 8") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 4, "Action 8 IT") & success;
    success = compareOutputs(mmval, 4, "Value 8 IT") & success;

    parseState("O.XOXX.O.OXXOX.XOXOX.O.OO.", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 3, "Action 9") & success;
    success = compareOutputs(mmval, 5, "Value 9") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 3, "Action 9 IT") & success;
    success = compareOutputs(mmval, 5, "Value 9 IT") & success;

    parseState("O.XOXX.O.OXXOXX.XOXOX.O.OOO.", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 6, "Action 10") & success;
    success = compareOutputs(mmval, 8, "Value 10") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 6, "Action 10 IT") & success;
    success = compareOutputs(mmval, 8, "Value 10 IT") & success;

    parseState("O.XOXXO.O.OXXOXX.XOXOX.O.OOOX.", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 4, "Action 11") & success;
    success = compareOutputs(mmval, 8, "Value 11") & success;
    action = solver::interative_deepening(state, myArgs, mmval);
    success = compareOutputs(action, 4, "Action 11 IT") & success;
    success = compareOutputs(mmval, 8, "Value 11 IT") & success;

    parseState("O.XOXXO.O.OXXOXX.XOXOXX.O.OOOXO", state);
    action = solver::minmax_search_standalone(state, 7, mmval);
    success = compareOutputs(action, 1, "Action 12") & success;
    success = compareOutputs(mmval, 7, "Value 12") & success;
    success = compareOutputs(action, 1, "Action 12 IT") & success;
    success = compareOutputs(mmval, 7, "Value 12 IT") & success;
    return success;
}

bool testHigherDepths(){
    bool success = true;
    GameState state;
    int mmval;
    int action;

    parseState("..XO.OXXO.O...", state);
    action = solver::minmax_search(state, 11, mmval);
    success = compareOutputs(action, 3, "Action 0") & success;
    success = compareOutputs(mmval, 1, "Value 0") & success;

    parseState("O.XOXX.O.OXXOX.XOXO.O.O.", state);
    action = solver::minmax_search(state, 17, mmval);
    success = compareOutputs(action, 4, "Action 0") & success;
    success = compareOutputs(mmval, 7, "Value 0") & success;

    parseState("O.XOXX.O.OXXOX.XOXO.O.O.", state);
    action = solver::minmax_search(state, 25, mmval);
    success = compareOutputs(mmval, -10, "Value 0") & success;

    return success;
}

// Run tests
int main() {
    for (int i = 0; i < 1000; i++) {
        if ((i ^ (i - 1)) == ((i << 1) - 1)) {
            std::cout << i << std::endl;
        }
    }

    bool succ = testState();
    std::cout << "Test State result: " << (succ ? "pass" : "fail") << std::endl;

    succ = testGameDepthSeven();
    std::cout << "Test GameDepthSeven result: " << (succ ? "pass" : "fail") << std::endl;
    succ = testCache();
    std::cout << "Test Cache result: " << (succ ? "pass" : "failed") << std::endl;
    succ = testHigherDepths();
    std::cout << "Test Higher Depths result: " << (succ ? "pass" : "fail") << std::endl;
    std::cout << "Press ENTER to exit." << std::endl;
    std::getchar();

}