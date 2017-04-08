//
//  TestGo.cpp
//  Go
//
//  Created by Ellis Hoag on 4/7/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestBoard
#include <boost/test/unit_test.hpp>

#include "Board.hpp"
#include <iostream>

BOOST_AUTO_TEST_CASE(testBoard) {
    
    const size_t numMoves = 4;
    int moves[2 * numMoves] = {5, 5,
                                  6, 5,
                                  6, 6,
                                  7, 7};
    
    Board myBoard;
    
    Player p = WHITE;
    for (size_t i = 0; i < 2 * numMoves; i += 2) {
        
        BOOST_REQUIRE(myBoard.move(p, moves[i], moves[i + 1]));
        p = (p == WHITE) ? BLACK : WHITE;
    }
}
