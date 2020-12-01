
#ifndef GAMESTATE
// Class to store a single state of the game. 
// Underlying data structure is a size 42 int array.
#include <string>
#include <cassert>
#include <cstdint>

class GameState
{
public:
    GameState();
    ~GameState();

private:
    // Store the values.
    int myState[42];

public:
    inline int get(int height, int x) const {
        if (x < 0 || x > 6 || height < 0 || height > 5) {
             assert(false);
        }
        // x in range [0,6] height in range[0,5], unchecked.
        int idx = 7 * height + x;
        return myState[idx];
    }
    
    inline void set(int height, int x, int val) {
        assert(-1 <= val && val <= 1);
        if (x < 0 || x > 6 || height < 0 || height > 5) {
            assert(false);
        }
        int idx = 7 * height + x;
        myState[idx] = val;
    }
    int getWinner() const;

    bool isComplete() const;

    bool applyAction(int player, int a);

    bool undoAction(int a);

    bool isActionValid(int a) const {
        return get(5, a) == 0;
    }

    bool isValidState() const;

    int rowsHeuristic() const;

    std::string toString() const;

    inline int getAnyValidAction() const {
        for (int i = 0; i < 7; i++) {
            if (isActionValid(i)) {
                return i;
            }
        }
        return 0;
    }

    /* Represent state by two long values. l1 has all bits set for fields with player1's chips.
    l2 has all bitsset for fields with chips of  player 2. l1 & l2 is always 0. */
    void getLongRep(int64_t &l1, int64_t &l2) const;

    void clear() {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 5; j++) {
                set(j, i, 0);
            }
        }
    }
    // Parse a game state from XO. notation, return true, if parse was successful.
    friend bool parseState(std::string arg1, GameState &state);

    bool operator == (const GameState &g2) const {
        int64_t l1, l2, ll21, ll22;
        getLongRep(l1, l2);
        g2.getLongRep(ll21, ll22);
        return l1 == ll21 && l2 == ll22;
    }


};

#endif // !GAMESTATE
#define GAMESTATE




