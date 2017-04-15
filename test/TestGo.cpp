//
//  TestGo.cpp
//  Go
//
//  Created by Ellis Hoag on 4/7/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testBoard
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <iostream>
#include <tuple>

/**
 *  HAHAHAHAHAHAHAHA
 *  C++ is a funny language.
 *  I want to test private methods.
 *  Hopefully this causes obscure bugs down the line.
 **/
#define private public
#include "Board.hpp"

static const int xLocs[] = {0, 4, 7, 1};
static const int yLocs[] = {0, 5, 1, 18};
BOOST_DATA_TEST_CASE(testSetStone, boost::unit_test::data::make(xLocs) ^ boost::unit_test::data::make(yLocs), x, y) {
    
    Board myBoard;
    
    myBoard._setStone(WHITE, x, y);
    BOOST_TEST(myBoard._getStoneOwner(x, y) == WHITE);
    myBoard._clearStone(x, y);
    BOOST_TEST(myBoard._getStoneOwner(x, y) == NEITHER);
}

BOOST_AUTO_TEST_CASE(testGetAdjacent) {
    
    Board myBoard;
    
    const size_t numMoves = 11;
    const int squares[2 * numMoves] = {
        0, 0,
        0, 1,
        5, 5,
        4, 5,
        5, 4,
        5, 6,
        6, 5,
        7, 5,
        8, 5,
        9, 5,
        9, 6
    };
    
    for (size_t i = 0; i < numMoves; i++) {
        myBoard._setStone(WHITE, squares[2 * i + 0], squares[2 * i + 1]);
    }
    
    BOOST_TEST(myBoard._getAdjacent(WHITE, 0, 0).size() == 1);
    BOOST_TEST(myBoard._getAdjacent(WHITE, 5, 5).size() == 4);
    auto connected = myBoard._getConnected(5, 5);
    BOOST_TEST(connected.size() == 9);
    BOOST_TEST(myBoard._getLiberties(connected).size() == 15);
}


BOOST_AUTO_TEST_CASE(testGame) {
    
    const size_t numMoves = 34;
    const int moves[2 * numMoves] = {
        2, 1,
        2, 2,
        5, 5,
        10, 10,
        4, 5,
        10, 11,
        5, 4,
        10, 12,
        5, 6,
        11, 10,
        6, 5,
        12, 10,
        7, 5,
        13, 11,
        8, 5,
        13, 12,
        9, 5,
        13, 13,
        9, 6,
        13, 14,
        2, 3,
        13, 15,
        3, 2,
        2, 19,
        1, 2,
        2, 4,
        14, 13,
        4, 4,
        14, 14,
        3, 3,
        19, 1,
        1, 3,
        19, 2,
        2, 2,
    };
    
    Board myBoard;
    
    for (size_t i = 0; i < 2 * numMoves; i += 2) {
        
        BOOST_REQUIRE(myBoard.move(moves[i], moves[i + 1]));
    }
    
    BOOST_REQUIRE(!myBoard.move(5, 5));
    
    auto connected = myBoard._getConnected(10 - 1, 10 - 1);
    BOOST_TEST(myBoard._getLiberties(connected).size() == 11);
    connected = myBoard._getConnected(13 - 1, 11 - 1);
    BOOST_TEST(myBoard._getLiberties(connected).size() == 10);
    
    for (size_t i = 0; i < numMoves; i++) {
        BOOST_REQUIRE(myBoard.undo());
    }
    
    BOOST_REQUIRE(!myBoard.undo());
    
    for (size_t i = 0; i < numMoves - 5; i++) {
        BOOST_REQUIRE(myBoard.redo());
    }
    
    BOOST_REQUIRE(myBoard.move(19, 19));
    BOOST_REQUIRE(!myBoard.redo());
    BOOST_REQUIRE(myBoard.move(19, 18));
    BOOST_REQUIRE(myBoard.move(19, 17));
    BOOST_REQUIRE(myBoard.move(19, 16));
    BOOST_REQUIRE(!myBoard.redo());
    
    auto score = myBoard.getScore();
    BOOST_TEST(std::get<0>(score) == 17);
    BOOST_TEST(std::get<1>(score) == 15);
    
    myBoard.pass();
    myBoard.pass();
    BOOST_TEST(myBoard.hasWon() == WHITE);
    
    BOOST_TEST(myBoard.exportSGF("test.sgf"));
}


BOOST_AUTO_TEST_CASE(testImportSGF) {
    
    Board myBoard;
    
    BOOST_REQUIRE(myBoard.importSGF("test.sgf"));
    
    BOOST_REQUIRE(!myBoard.move(5, 5));
    
    auto connected = myBoard._getConnected(10 - 1, 10 - 1);
    BOOST_TEST(myBoard._getLiberties(connected).size() == 11);
    connected = myBoard._getConnected(13 - 1, 11 - 1);
    BOOST_TEST(myBoard._getLiberties(connected).size() == 10);
    
    myBoard.pass();
    myBoard.pass();
    BOOST_TEST(myBoard.hasWon() == WHITE);
}

