#ifndef MOVECACHE_H

// A Cache storing moves that were already visited.
// The underlying data structure is a hashmap (std::unordered:map).

#include <unordered_map>
#include <utility>
#include <string>
#include <cstdint>

// Hash function for int_64t
template<>
struct std::hash<std::pair<int64_t, int64_t>> {
public:
    size_t operator()(const std::pair<int64_t, int64_t> &s) const
    {
        size_t h1 = std::hash<int64_t>()(s.first);
        size_t h2 = std::hash<int64_t>()(s.second);
        return h1 ^ (h2 << 1);
    }
};

class GameState;
class MoveCache
{
    // Store minmax values for certain states
public:
    MoveCache() = default;
    ~MoveCache() = default;

    /* For an state it is not enough to only store the alpha-beta value.
    Additionally, we store the alpha value if this is a MinNode (its Player -1's turn) and beta if this is a Max Node (it's Player 1's turn).
    */
    struct MoveEntry {
        bool isMaxNode;
        int alphabeta;      // Store the alpha value for maxNodes, betaValue for minNodes
                            // A lower Alpha value is more general than a larger, a bigger beta value is more general then a smaller.
        int minmaxval;
    };

    /* Return true, if a suitable value is in the cache */
    bool isHit(const GameState & state, int alphabeta) const;

    /* Update a value or add a new one. */
    void addState(const GameState& state, bool isMaxNode, int alphabeta, int minmaxval);

    /* Get the value for a state. Use only after applicability and existance of value 
    has been checked by isHit(). Returns 0 if the state is not found in cache.*/
    int getMinMaxVal(const GameState& state) const;

    size_t getCacheSize() const {
        return myCache.size();
    }

    void toFile(std::string fname) const;

    // Remove all states that are non terminals
    void clearAllNonTerms();

    int getHits() {
        return hitcount;
    }

private:

    // Moves will be stored as std::pair of long long
    std::unordered_map<std::pair<int64_t, int64_t>, MoveEntry> myCache;

    mutable int hitcount = 0;

};
#endif // !MOVECACHE_H
#define MOVECACHE_H

