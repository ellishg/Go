//
//  History.cpp
//  Go
//
//  Created by Ellis Hoag on 4/7/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "History.hpp"


void History::clear() {
    moveList.clear();
    iCurrentMove = 0;
}

void History::pushMove(move_t move) {
    
    moveList.erase(moveList.begin() + iCurrentMove, moveList.end());
    
    moveList.push_back(move);
    
    iCurrentMove++;
}

move_t History::popMove() {
    
    if (iCurrentMove == 0) {
        //No previous move
        return move_t();
    }
    
    return moveList[--iCurrentMove];
}

move_t History::nextMove() {
    
    if (iCurrentMove == moveList.size()) {
        //No next move
        return move_t();
    }
    
    return moveList[++iCurrentMove];
}

