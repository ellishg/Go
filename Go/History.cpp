//
//  History.cpp
//  Go
//
//  Created by Ellis Hoag on 4/7/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "History.hpp"

History & History::operator=(const History & other) {
    
    if (this != &other) {
        
        moveList = other.moveList;
        iNextMove = other.iNextMove;
    }
    
    return *this;
}

void History::clear() {
    moveList.clear();
    iNextMove = 0;
}

std::vector<move_t> History::getMoves() {
    
    return std::vector<move_t>(moveList.begin(), moveList.begin() + iNextMove);
}

void History::pushMove(move_t move) {
    
    moveList.erase(moveList.begin() + iNextMove, moveList.end());
    
    moveList.push_back(move);
    
    iNextMove++;
}

move_t History::popMove() {
    
    if (iNextMove == 0) {
        //No previous move
        return move_t();
    }
    
    return moveList[--iNextMove];
}

move_t History::nextMove() {
    
    if (iNextMove == moveList.size()) {
        //No next move
        return move_t();
    }
    
    return moveList[iNextMove++];
}

