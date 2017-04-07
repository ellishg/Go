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

#include "Move.hpp"
#include "History.hpp"

class Board {
    
private:
    
    size_t width, height;
    
    Color** board;
    
    History history;
        
    void _clearBoard();
    
    void _setSquare(size_t x, size_t y);
    
    void _clearSquare(size_t x, size_t y);
    
    void _applyMove(move_t move);
    
    void _repealMove(move_t antiMove);
    
    bool _isValidPlacement(Color player, size_t x, size_t y);
    
    move_t _generateMove(Color player, size_t x, size_t y);
    
    size_t _countLiberties();
    
    std::vector<std::tuple<size_t, size_t>> _getLiberties();
    
public:
    
    Board(size_t _width = 19, size_t _height = 19);
    
    ~Board();
    
    void clear();
    
    bool move(Color player, size_t x, size_t y);
    
    bool undo();
    
    bool redo();
    
    void passMove(Color player);
    
    std::tuple<size_t, size_t> getScore();
};


#endif /* Board_hpp */
