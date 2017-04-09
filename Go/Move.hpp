//
//  Move.hpp
//  Go
//
//  Created by Ellis Hoag on 4/7/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Move_h
#define Move_h

#include <vector>
#include <tuple>


enum Player {
    WHITE = 0b10,
    BLACK = 0b01,
    NEITHER = 0b00
};


struct move_t {
    
    move_t() : mover(NEITHER), placedStone(std::make_tuple(-1, -1)) {}
    
    Player mover;
    
    std::tuple<int, int> placedStone;
    
    std::vector<std::tuple<int, int>> killedStones;
    
    /**
     *  @return True if the move is defined to be valid.
     */
    bool isValid() {
        return mover != NEITHER;
    }
};


#endif /* Move_h */
