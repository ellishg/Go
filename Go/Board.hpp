//
//  Board.hpp
//  Go
//
//  Created by Ellis Hoag on 4/5/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <vector>
#include <tuple>


enum Color {
    WHITE = 0b10,
    BLACK = 0b01,
    NEITHER = 0b00
};


struct move_t {
    
    Color mover;
    
    std::tuple<size_t, size_t> placedPiece;
    
    std::vector<std::tuple<size_t, size_t>> killedPieces;
};


class Board {
    
private:
    
    size_t width, height;
    
    Color** board;
    
    std::vector<move_t> history;
    
    void _setSquare(size_t x, size_t y);
    
    void _clearSquare(size_t x, size_t y);
    
    void _applyMove(move_t move);
    
    void _repealMove(move_t move);
    
    bool _isValidMove(move_t move);
    
    size_t _countLiberties();
    
    std::vector<std::tuple<size_t, size_t>> _getLiberties();
    
public:
    
    Board(size_t _width = 19, size_t _height = 19);
    
    ~Board();
    
    void clearBoard();
    
    bool move(Color player, size_t x, size_t y);
    
    bool undo();
    
    bool redo();
    
    void passMove(Color player);
    
    std::tuple<size_t, size_t> getScore();
};


#endif /* Board_hpp */
