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


enum Color {
    WHITE = 0b10,
    BLACK = 0b01,
    NEITHER = 0b00
};


struct move_t {
    
    move_t() : mover(NEITHER) {}
    
    Color mover;
    
    std::tuple<size_t, size_t> placedPiece;
    
    std::vector<std::tuple<size_t, size_t>> killedPieces;
    
    bool isValid() {
        return mover != NEITHER;
    }
};


#endif /* Move_h */
