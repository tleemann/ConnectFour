#ifndef SOLVER
#define REWARD_WIN 10
#define REWARD_LOSE -10
#define REWARD_DRAW 1

#define VERBOSE false
#define USE_CACHE true
#define AB_PRUNE true

#include <algorithm>
#include <iostream>
#include <chrono>

#include "GameState.h"
#include "Globals.h"
#include "MoveCache.h"

// A static solver class that performs an alpha-beta pruning search with caching for the connect four game

using namespace std;
namespace solver {

    struct ConnectFourArgs {
        bool verbose = false;
        int maxdepth = 5;
        int maxtime = 1000;
    };

    static const int moveOrder[7] = {3, 4, 2, 5, 1, 0, 6};

    static int min_rec(GameState& state, int &ret_action, int depth_left, int alpha, int beta);
    static int max_rec(GameState& state, int &ret_action, int depth_left, int alpha, int beta);
    static int minmax_search(GameState &start, int depth, int& minmaxval);

    static MoveCache myCache = MoveCache();

    // Returns the best move
    static int interative_deepening(GameState &state, const ConnectFourArgs &myArgs, int mmval) {
        if (myArgs.verbose) {
            globvars::verbose = true;
        } 
        myCache = MoveCache();
        int action = state.getAnyValidAction();
        int mydep = 0; 
        int passedTime = 0;
        while(mydep <= myArgs.maxdepth && passedTime < (myArgs.maxtime/3)){
            auto start = chrono::steady_clock::now();
            action = minmax_search(state, mydep, mmval);
            auto end = chrono::steady_clock::now();
            passedTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
            //std::cerr << "passedtime " << passedTime << std::endl;
            mydep++;
        }
        if (globvars::verbose) {
            std::cerr << "Reached depth: " << mydep-1 << std::endl;
        }
        return action;
    }


    /** Returns the best move **/
    static int minmax_search(GameState &start, int depth, int& minmaxval) {
        GameState copystart = start;
        myCache.clearAllNonTerms();
        //resCache = MoveCache();
        //myCache = MoveCache();

        int action = -1;
        minmaxval = max_rec(copystart, action, depth, REWARD_LOSE, REWARD_WIN);

        if (globvars::verbose) {
            std::cerr << "Got minmax value of " << minmaxval << std::endl;
            std::cerr << "Cache size: " << myCache.getCacheSize() << " Hits: " << myCache.getHits() << std::endl;
        }
        //myCache.toFile("movecache");
        return action;
    }

    static int minmax_search_standalone(GameState &start, int depth, int& minmaxval) {
        // Clear cache
        myCache = MoveCache();
        return minmax_search(start, depth, minmaxval);
    }

    /**
    Min node of the alpha-beta tree search. 
    Alpha is the highest max value encountered.
    Beta is the lowest min value encountered.
    If we encounter a value that is lower than the current alpha, we can be certain that this
    node wont be used in the node above and can stop searching.
    */
    static int min_rec(GameState& state, int &ret_action, int depth_left, int alpha, int beta) {

        if (USE_CACHE && myCache.isHit(state, beta)) {
            int val = myCache.getMinMaxVal(state);
            if (val <= alpha) {
                return val;
            }
        }
        ret_action = 0;
        if (VERBOSE && depth_left >= 20){
            cerr << "Entered min Node " << depth_left << " in state: \n";
            cerr << state.toString();
        }
        if (state.isComplete()) {
            return REWARD_DRAW;
        }
        int w = state.getWinner();
        if (w != 0) {
            if (w == 1) {
                return REWARD_WIN;
            }
            else if (w == -1) {
                return REWARD_LOSE;
            }
        }
        if (depth_left == 0) {
            int val = state.rowsHeuristic();
            myCache.addState(state, false, -REWARD_WIN, val);
            return val;
        }
        // Not over yet.
        int min_val = REWARD_WIN+1;
        for (auto i = 0; i < 7; i++) { 
            if (state.isActionValid(moveOrder[i])) {
                state.applyAction(-1, moveOrder[i]);
 
                int action = 0;
                int ret = max_rec(state, action, depth_left - 1, alpha, beta);
                if (ret < min_val) {
                    ret_action = moveOrder[i];
                    min_val = ret;
                    if (VERBOSE) {
                        cerr << "Update: Level " << depth_left << " min_val= "<< min_val <<" \n";
                    }
                }
                if ((AB_PRUNE && ret <= alpha) || ret == REWARD_LOSE) {
                    ret_action = moveOrder[i];
                    state.undoAction(moveOrder[i]);
                    if (VERBOSE && depth_left >= 20) {
                        cerr << "Update: Level " << depth_left << " return min_val= " << min_val << " \n";
                    }
                    myCache.addState(state, false, beta, min_val);
                    return ret;
                }
                beta = std::min(beta, ret);
                state.undoAction(moveOrder[i]);
            }
        }
        if (VERBOSE && depth_left >= 20) {
            cerr << "Update: Level " << depth_left << " return min_val= " << min_val << " \n";
        }

        myCache.addState(state, false, beta, min_val);
         return min_val;
    }

    /**
    Max node of the alpha-beta tree search.
    alpha is the highest max value encountered.
    beta is the lowest min value encountered.
    If we encounter a value that is higher than the current beta, we can be certain that this
    node wont be used in the node above and can stop searching.
    */ 
    static int max_rec(GameState& state, int &ret_action, int depth_left, int alpha, int beta) {
        if (USE_CACHE && myCache.isHit(state, alpha) && ret_action >= 0) {
            int val = myCache.getMinMaxVal(state);
            if (val >= beta) {
                return val;
            }
        }

        ret_action = 0;
        if (VERBOSE && depth_left >= 20) {
            cerr << "Entered max Node " << depth_left << " in state: \n";
            cerr << state.toString();
        }
        if (state.isComplete()) {
            return REWARD_DRAW;
        }
        int w = state.getWinner();
        if (w != 0) {
            if (w == 1) {
                return REWARD_WIN;
            }
            else if (w == -1) {
                return REWARD_LOSE;
            }
        }
        if (depth_left == 0) {
            int val = state.rowsHeuristic();
            myCache.addState(state, true, -REWARD_LOSE, val);
            return val;
        }
        // Not over yet.
        int max_val = REWARD_LOSE-1;
        for (auto i = 0; i < 7; i++) {
            if (state.isActionValid(moveOrder[i])) {
                state.applyAction(1, moveOrder[i]);
                int action = 0;
                int ret = min_rec(state, action, depth_left - 1, alpha, beta);
                if (ret > max_val) {
                    ret_action = moveOrder[i];
                    max_val = ret;
                    if (VERBOSE) {
                        //cerr << "Update: Level " << depth_left << " max_val= " << max_val << " \n";
                    }
                }
                if ((AB_PRUNE && max_val >= beta) || max_val == REWARD_WIN) {
                    ret_action = moveOrder[i];
                    state.undoAction(moveOrder[i]);
                    if (VERBOSE && depth_left >= 20) {
                        cerr << "Update: Level " << depth_left << " return max_val= " << max_val << " \n";
                    }
                    myCache.addState(state, true, alpha, max_val);
                    return max_val;
                }
                alpha = std::max(alpha, max_val);
                state.undoAction(moveOrder[i]);
            }
        }
        if (VERBOSE && depth_left >= 20) {
            cerr << "Update: Level " << depth_left << " return max_val= " << max_val << " \n";
        }

        myCache.addState(state, true, alpha, max_val);
        return max_val;
    }
}
#endif
#define SOLVER
