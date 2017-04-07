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

#include "Move.hpp"


class History {
    
private:
    
    std::vector<move_t> moveList;
    
    size_t iCurrentMove;
    
public:
    
    History();
    
    ~History();
    
    void clear();
    
    void pushMove(move_t move);
    
    move_t popMove();
    
    move_t nextMove();
};


#endif /* History_hpp */
