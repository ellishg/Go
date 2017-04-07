//
//  Board.cpp
//  Go
//
//  Created by Ellis Hoag on 4/5/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Board.hpp"

using namespace std;

Board::Board(size_t _width, size_t _height) {
    
    width = _width;
    height = _height;
    
    clear();
}

Board::~Board() {
    
}

void Board::clear() {
    
    history.clear();
    
    _clearBoard();
}

bool Board::move(Color player, size_t x, size_t y) {
    
    if (_isValidPlacement(player, x, y)) {
    
        move_t move = _generateMove(player, x, y);

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
    
    return false;
}

void Board::passMove(Color player) {
    
}

tuple<size_t, size_t> Board::getScore() {
    
    return make_tuple(42, 42);
}
