//
//  Board.cpp
//  Go
//
//  Created by Ellis Hoag on 4/5/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Board.hpp"

using namespace std;

Board::Board(int _width, int _height, bool _prohibitSuicide) {
    
    width = _width;
    height = _height;
    prohibitSuicide = _prohibitSuicide;
    
    board = new Player[width * height];
    
    clear();
}

Board::~Board() {
    delete [] board;
}

void Board::clear() {
    
    history.clear();
    
    _clearBoard();
}

bool Board::move(Player p, int x, int y) {
    
    move_t move = _generateMove(p, x, y);

    if (move.isValid()) {
        
        _applyMove(move);
                
        return true;
    }
    
    return false;
}

bool Board::undo() {
    
    move_t antiMove = history.popMove();
    
    if (!antiMove.isValid()) {
        return false;
    }
    
    _repealMove(antiMove);
    
    return true;
}

bool Board::redo() {
    
    move_t move = history.nextMove();
    
    if (!move.isValid()) {
        return false;
    }
    
    _applyMove(move);
    
    return true;
}

void Board::pass(Player p) {
    
}

set<tuple<int, int>> Board::_getAdjacent(Player p, int x, int y) {
    
    set<tuple<int, int>> neighbors;
    
    const int locs[8] = {
        x    , y - 1,
        x + 1, y,
        x    , y + 1,
        x - 1, y
    };
    
    for (size_t i = 0; i < 4; i++) {
        int u = locs[2 * i + 0];
        int v = locs[2 * i + 1];
        if (_isOnBoard(u, v) && _getStoneOwner(u, v) == p) {
            neighbors.insert(tuple<int, int>(u, v));
        }
    }
    
    return neighbors;
}

/**
 *  In a given position, a liberty of a stone is an empty
 *  intersection adjacent to that stone or adjacent to a
 *  stone which is connected to that stone.
 */
set<tuple<int, int>> Board::_getLiberties(set<tuple<int, int>> connected) {
    
    set<tuple<int, int>> liberties;
    
    for (auto & stone : connected) {
        
        auto neighbors = _getAdjacent(NEITHER, get<0>(stone), get<1>(stone));
        
        liberties.insert(neighbors.begin(), neighbors.end());
    }
    
    return liberties;
}


/**
 *  Two placed stones of the same color (or two empty intersections)
 *  are said to be connected if it is possible to draw a path from
 *  one intersection to the other by passing through adjacent
 *  intersections of the same state (empty, occupied by white, or
 *  occupied by black).
 */
set<tuple<int, int>> Board::_getConnected(int x, int y) {
    
    Player owner = _getStoneOwner(x, y);
    
    if (!_isOnBoard(x, y) || owner == NEITHER) {
        return set<tuple<int, int>>();
    }
 
    set<tuple<int, int>> connected;
    
    queue<tuple<int, int>> bag;

    bag.push(make_tuple(x, y));

    while (!bag.empty()) {
        auto stone = bag.front();
        bag.pop();
        
        if (connected.find(stone) != connected.end()) {
            connected.insert(stone);
            
            auto neighbors = _getAdjacent(owner, get<0>(stone), get<1>(stone));
            
            for (auto & neighbor : neighbors) {
                bag.push(neighbor);
            }
        }
    }
    
    return connected;
}

tuple<int, int> Board::getScore() {
    
    return make_tuple(42, 42);
}

void Board::_clearBoard() {
    for (int i = 0; i < width * height; i++) {
        board[i] = NEITHER;
    }
}

void Board::_setStone(Player p, int x, int y) {
    if (_isOnBoard(x, y)) {
        board[x + width * y] = p;
    }
}

Player Board::_getStoneOwner(int x, int y) {
    if (!_isOnBoard(x, y)) {
        return NEITHER;
    }
    return board[x + width * y];
}

bool Board::_isOnBoard(int x, int y) {
    
    return (0 <= x && x < width &&
            0 <= y && y < height);
}

void Board::_clearStone(int x, int y) {
    if (_isOnBoard(x, y)) {
        board[x + width * y] = NEITHER;
    }
}

void Board::_applyMove(move_t move) {
    
    int x = get<0>(move.placedStone);
    int y = get<1>(move.placedStone);
    
    _setStone(move.mover, x, y);
    
    for (auto killed : move.killedStones) {
        _clearStone(get<0>(killed), get<1>(killed));
    }
}

void Board::_repealMove(move_t antiMove) {
    
    int x = get<0>(antiMove.placedStone);
    int y = get<1>(antiMove.placedStone);
    
    _clearStone(x, y);
    
    Player opponent = (antiMove.mover == WHITE) ? BLACK : WHITE;
    
    for (auto recovered : antiMove.killedStones) {
        _setStone(opponent, get<0>(recovered), get<1>(recovered));
    }
}

/**
 *  Step 1: Removing from the board any stones of their opponent's
 *          color that have no liberties.
 *  Step 2: Removing from the board any stones of their own color
 *          that have no liberties.
 *  Step 3: (Optional) Prohibition of suicide: A play is illegal
 *          if one or more stones of that player's color would be
 *          removed in Step 2 of that play.
 *  Step 4: A play is illegal if it would have the effect (after
 *          all steps of the play have been completed) of creating
 *          a position that has occurred previously in the game.
 *
 */
move_t Board::_generateMove(Player p, int x, int y) {
    
    if (_isOnBoard(x, y) && p != NEITHER) {
        if (_getStoneOwner(x, y) == NEITHER) {
            
            move_t move;
            move.mover = p;
            move.placedStone = make_tuple(x, y);
            
            Player opponent = (p == WHITE) ? BLACK : WHITE;
            
            _setStone(p, x, y);
            
            auto neighboringFoes = _getAdjacent(opponent, x, y);
            auto neighboringFriends = _getAdjacent(p, x, y);
            neighboringFriends.insert(make_tuple(x, y));
            
            for (auto & stone : neighboringFoes) {
                auto connected = _getConnected(get<0>(stone), get<1>(stone));
                auto liberties = _getLiberties(connected);
                
                if (liberties.empty()) {
                    for (auto & killed : connected) {
                        move.killedStones.push_back(killed);
                    }
                }
            }
            
            for (auto & stone : neighboringFriends) {
                auto connected = _getConnected(get<0>(stone), get<1>(stone));
                auto liberties = _getLiberties(connected);
                
                if (liberties.empty()) {
                    if (prohibitSuicide) {
                        _clearStone(x, y);
                        return move_t();
                    }
                    else {
                        for (auto & killed : connected) {
                            move.killedStones.push_back(killed);
                        }
                    }
                }
            }
            
            _clearStone(x, y);
            return move;
        }
    }
    
    return move_t();
}
