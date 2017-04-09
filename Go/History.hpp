//
//  History.hpp
//  Go
//
//  Created by Ellis Hoag on 4/7/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef History_hpp
#define History_hpp

#include <vector>
#include <stdlib.h>

#include "Move.hpp"


class History {
    
private:
    
    std::vector<move_t> moveList;
    
    size_t iNextMove;
    
public:
    
    History() {clear();}
    
    History & operator=(const History & other);
    
    /**
     *  Clears the history.
     */
    void clear();
    
    /**
     *
     *  @param move The move that has just occured
     *                  to be added to the history.
     */
    void pushMove(move_t move);
    
    /**
     *  @return The previous move in the history.
     */
    move_t popMove();
    
    /**
     *  @return The next move in the history.
     */
    move_t nextMove();
    
    /**
     *  @return An ordered vector of moves.
     */
    std::vector<move_t> getMoves();
};


#endif /* History_hpp */
