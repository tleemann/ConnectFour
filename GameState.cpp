#include "GameState.h"
#include "Globals.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

GameState::GameState()
{
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 7; i++) {
            set(j, i, 0);
        }
    }
}


GameState::~GameState()
{

}

int GameState::getWinner() const
{
    // Search rows
    for (auto i = 0; i < 6; i++) {
        int row_len = 0;
        int player = 0;
        for (auto j = 0; j < 7; j++) {
            if (get(i, j) != 0) {
                if (player == get(i, j)) {
                    row_len++;
                    if (row_len == 4) {
                        return player;
                    }
                }
                else {
                    player = get(i, j);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
            }
        }
    }
    // columns columns
    for (auto i = 0; i < 7; i++) {
        int row_len = 0;
        int player = 0;
        for (auto j = 0; j < 6; j++) {
            if (get(j, i) != 0) {
                if (player == get(j, i)) {
                    row_len++;
                    if (row_len == 4) {
                        return player;
                    }
                }
                else {
                    player = get(j, i);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
            }
        }
    }

    // diagonal up;
    for (auto i = -2; i < 4; i++) { // -2, -1, 0, 1, 2, 3
        int row_len = 0;
        int player = 0;
        for (auto j = std::max(0, -i); i + j < 7 && j < 6; j++) {
            if (get(j, i + j) != 0) {
                if (player == get(j, i + j)) {
                    row_len++;
                    if (row_len == 4) {
                        return player;
                    }
                }
                else {
                    player = get(j, i + j);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
            }
        }
    }

    // Diagonal down
    for (auto i = -2; i < 4; i++) { // -2, -1, 0, 1, 2, 3
        int row_len = 0;
        int player = 0;
        for (auto j = std::max(0, -i); 5 - j >= 0 && i + j < 7; j++) {
            if (get(5 - j, i + j) != 0) {
                if (player == get(5 - j, i + j)) {
                    row_len++;
                    if (row_len == 4) {
                        return player;
                    }
                }
                else {
                    player = get(5 - j, i + j);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
            }
        }
    }
    return 0;
}

bool GameState::isComplete() const
{
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            if (get(j,i) == 0) {
                return false;
            }
        }
    }
    return true;
}

bool GameState::applyAction(int player, int a)
{
    // a in range [0,5] unchecked
    if (get(5, a)) {
        return false;
    }
    else {
        auto i = 0;
        while (get(i, a) != 0) {
            i++;
        }
        set(i, a, player);
    }
    return true;
}

bool GameState::undoAction(int a)
{
    auto i = 5;
    while (i >= 0 && get(i, a) == 0) {
        i--;
    }
    if (i == -1) {
        return false;
    }
    else {
        set(i, a, 0);
    }
    return true;
}

bool GameState::isValidState() const
{
    // Valid state: X has as many pieces as O ore one piece less
    auto oPieces = 0;
    auto xPieces = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (get(j, i) == 1) {
                xPieces++;
            }
            else if (get(j, i) == -1) {
                oPieces++;
            }
            else if (get(j, i) != 0) {
                if (globvars::verbose) {
                    std::cout << "Illegal value " << get(i, j) << "encountered.\n";
                }
                return false;
            }
        }
    }
    if (xPieces > oPieces || xPieces < oPieces - 1) {
        if (globvars::verbose) {
            std::cout << "Illegal piece count " << xPieces << " : " << oPieces <<  "encountered.\n";
        }
        return false;
    }

    // Columns are filled up from bottom to top
    for (auto i = 0; i < 7; i++) {
        bool done = false;
        for (auto j = 0; j < 6; j++) {
            if (get(j, i) != 0) {
                if (done) { // We encountered a done
                    return false;
                }
            }
            else {
                done = true;
            }
        }
    }
    return true;
}

// Add 1 point for every own two in a row
// Add 3 point for every own three in a row
// Subtract 1/3 for the opponent.
int GameState::rowsHeuristic() const {
    const int reward2 = 1;
    const int reward3 = 2; //Additional
    int value = 0;
    // Search rows
    for (auto i = 0; i < 6; i++) {
        int row_len = 0;
        int player = 0;
        bool beginfree = false; // field before row is empty
        for (auto j = 0; j < 7; j++) {
            if (get(i, j) != 0) {
                if (player == get(i, j)) {
                    row_len++;
                    if (row_len == 2 && (beginfree || (j < 6 && get(i, j + 1)==0))) {
                        value += player*reward2;
                    }
                    if (row_len == 3 && (beginfree || (j < 6 && get(i, j + 1) == 0))) {
                        value += player*reward3;
                    }
                }
                else {
                    // Sudden change of color
                    if (player != 0) {
                        beginfree = false;
                    }
                    player = get(i, j);
                    row_len = 1;

                }
            }
            else {
                player = 0;
                row_len = 0;
                beginfree = true;
            }
        }
    }

    // columns columns
    for (auto i = 0; i < 7; i++) {
        int row_len = 0;
        int player = 0;
        bool beginfree = false;
        for (auto j = 0; j < 6; j++) {
            if (get(j, i) != 0) {
                if (player == get(j, i)) {
                    row_len++;
                    row_len++;
                    if (row_len == 2 && (beginfree || (j < 5 && get(j + 1, i) == 0))) {
                        value += player*reward2;
                    }
                    if (row_len == 3 && (beginfree || (j < 5 && get(j + 1, i) == 0))) {
                        value += player*reward3;
                    }
                }
                else {
                    if (player != 0) {
                        beginfree = false;
                    }
                    player = get(j, i);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
                beginfree = true;
            }
        }
    }

    // diagonal up;
    for (auto i = -2; i < 4; i++) { // -2, -1, 0, 1, 2, 3
        int row_len = 0;
        int player = 0;
        bool beginfree = false;
        for (auto j = std::max(0, -i); i + j < 7 && j < 6; j++) {
            if (get(j, i + j) != 0) {
                if (player == get(j, i + j)) {
                    row_len++;
                    if (row_len == 2 && (beginfree || (i + j + 1 < 7  && j + 1 < 6 && get(j + 1, i + j + 1) == 0))) {
                        value += player*reward2;
                    }
                    if (row_len == 3 && (beginfree || (i + j + 1 < 7 && j + 1 < 6 && get(j + 1, i + j + 1) == 0))) {
                        value += player*reward3;
                    }
                }
                else {
                    if (player != 0) {
                        beginfree = false;
                    }
                    player = get(j, i + j);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
                beginfree = true;
            }
        }
    }

    // Diagonal down
    for (auto i = -2; i < 4; i++) { // -2, -1, 0, 1, 2, 3
        int row_len = 0;
        int player = 0;
        bool beginfree = false;
        for (auto j = std::max(0, -i); 5 - j >= 0 && i + j < 7; j++) {
            if (get(5 - j, i + j) != 0) {
                if (player == get(5 - j, i + j)) {
                    row_len++;
                    if (row_len == 2 && (beginfree || (5 - (j + 1) >= 0 && i + j + 1 < 7 && get(5 - (j + 1), i + j + 1) == 0))) {
                        value += player*reward2;
                    }
                    if (row_len == 3 && (beginfree || (5 - (j + 1) >= 0 && i + j + 1 < 7 && get(5 - (j + 1), i + j + 1) == 0))) {
                        value += player*reward3;
                    }
                }
                else {
                    if (player != 0) {
                        beginfree = false;
                    }
                    player = get(5 - j, i + j);
                    row_len = 1;
                }
            }
            else {
                player = 0;
                row_len = 0;
                beginfree = true;
            }
        }
    }
    return std::max(std::min(value,9),-9); // Limit range [-9,9]
}

std::string GameState::toString() const
{
    stringstream ss;
    for (auto j = 5; j >=0; j--) {
        ss << "<";
        for (auto i = 0; i < 7; i++) {
            if (get(j, i) == 0) {
                ss << " ";
            }
            else if (get(j, i) == 1) {
                ss << "X";
            }
            else {
                ss << "O";
            }
        }
        ss << "<";
        ss << "\n";
    }
    return ss.str();
}


void GameState::getLongRep(int64_t &l1, int64_t  &l2) const
{
    l1 = (int64_t)0;
    l2 = (int64_t)0;
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 7; i++) {
            if (get(j, i) == 1) {
                int idx = j * 7 + i;
                l1 += ((int64_t)1 << idx);
            }
            else if (get(j, i) == -1) {
                int idx = j * 7 + i;
                l2 += ((int64_t)1 << idx);
            }
        }
    }
}

bool parseState(std::string arg1, GameState &state){
    state.clear();
    int i = 0;
    int clm = 0;
    int row = 0;
    while (arg1[i] != 0) {
        if (arg1[i] == '.') {
            clm++;
            row = 0;
            if (clm > 7) {
                return false;
            }
        }
        else if (arg1[i] == 'X') {
            if (row >= 6 || clm >= 7) {
                return false;
            }
            state.set(row, clm, 1);
            row++;
        }
        else if (arg1[i] == 'O') {
            if (row >= 6 || clm >= 7) {
                return false;
            }
            state.set(row, clm, -1);
            row++;
        }
        else {
            if (globvars::verbose) {
                std::cout << "Illegal character in state arg: " << arg1[i] << std::endl;
            }
            return false;
        }
        i++;
    }
    return true;
}

