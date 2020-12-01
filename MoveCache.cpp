#include "MoveCache.h"
#include "GameState.h"
#include <iostream>
#include <fstream>


bool MoveCache::isHit(const GameState & state, int alphabeta) const
{   
    //return false;
    int64_t l1, l2;
    state.getLongRep(l1, l2);
    auto it = myCache.find(std::pair<int64_t, int64_t>(l1, l2));
    if (it != myCache.end()) {
        if (it->second.isMaxNode) { // alpha is stored.
            if (it->second.alphabeta <= alphabeta) {
                hitcount++;
                return true;
            }
        }
        else { // beta is stored.
            if (it->second.alphabeta >= alphabeta) {
                hitcount++;
                return true;
            }
        }
    }
    return false;
}


void MoveCache::addState(const GameState & state, bool isMaxNode, int alphabeta, int minmaxval)
{
    int64_t l1, l2;
    state.getLongRep(l1, l2);
    if (myCache.find(std::pair<int64_t, int64_t>(l1, l2)) == myCache.end()) {
        myCache.emplace(std::pair<int64_t, int64_t>(l1, l2), MoveEntry{ isMaxNode, alphabeta, minmaxval });
    }
    else {
        auto it = myCache.find(std::pair<int64_t, int64_t>(l1, l2));
        // Check if the new value is more general than the old one.
        if (it->second.isMaxNode) {
            // To be more general: Either a lower alpha beta value 
            if (it->second.alphabeta > alphabeta) {
                it->second = MoveEntry{ isMaxNode, alphabeta, minmaxval };
            }
            if (it->second.alphabeta == alphabeta && it->second.minmaxval < minmaxval) {
                it->second = MoveEntry{ isMaxNode, alphabeta, minmaxval };
            }
        }
        else { // Min node
            if (it->second.alphabeta < alphabeta) {
                it->second = MoveEntry{ isMaxNode, alphabeta, minmaxval };
            }
            if (it->second.alphabeta == alphabeta && it->second.minmaxval > minmaxval) {
                it->second = MoveEntry{ isMaxNode, alphabeta, minmaxval };
            }
        }
    }
}

int MoveCache::getMinMaxVal(const GameState & state) const
{
    int64_t l1, l2;
    state.getLongRep(l1, l2);
    auto it = myCache.find(std::pair<int64_t, int64_t>(l1, l2));
    if (it != myCache.end()) {
        return it->second.minmaxval;
    }
    return 0;
}

void MoveCache::toFile(std::string fname) const
{
    std::ofstream output;
    output.open(fname, std::ios::out);
    for (auto &it : myCache) {
        output << it.first.first << " " << it.first.second << " (" << it.second.alphabeta << ", " << it.second.minmaxval << ")\n";
    }
    output.flush();
    output.close();
}

void MoveCache::clearAllNonTerms()
{
    for (auto it = myCache.begin(); it != myCache.end();) {
        if (it->second.minmaxval != -10 && it->second.minmaxval != 10) {
            it = myCache.erase(it);
        }
        else {
            ++it;
        }
    }
}


