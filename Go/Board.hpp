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
#include <set>
#include <queue>

#include "Move.hpp"
#include "History.hpp"

class Board {
    
private:
    
    int width, height;
    
    Player* board;
    
    Player nextToMove;
    
    Player didJustPass;
    
    History history;
    
    bool prohibitSuicide;
    
    /**
     *  Clears the stones on the board.
     */
    void _clearBoard();
    
    /**
     *  Player p places a stone at square (x, y).
     *  @param p The player is placing the peice.
     *  @param x The x location of the peice.
     *  @param y The y location of the stone.
     */
    void _setStone(Player p, int x, int y);
    
    /**
     *  The stone at square (x, y) to be removed.
     *  @param x The x location of the peice.
     *  @param y The y location of the stone.
     */
    void _clearStone(int x, int y);
    
    /**
     *  @param x The x location of the stone.
     *  @param y The y location of the stone.
     *  @return The Player that owns square (x, y) or
     *              NEITHER if the square is empty.
     */
    Player _getStoneOwner(int x, int y);
    
    /**
     *  @param p The owner of the neighboring stones to find.
     *  @param x The x location of a square.
     *  @param y The y location of a square.
     *  @return A set of tuples of two integers representing
     *              the location of stones adjacent to square
     *              (x, y) that are owned by p.
     */
    std::set<std::tuple<int, int>> _getAdjacent(Player p, int x, int y);
    
    /**
     *  @param x The x location to test.
     *  @param y The y location to test.
     *  @return True if square (x, y) is on the board.
     */
    bool _isOnBoard(int x, int y);
    
    /**
     *  @param move The move to be applied.
     */
    void _applyMove(move_t move);
    
    /**
     *  @param antiMove The move to be repealed.
     */
    void _repealMove(move_t antiMove);
    
    /**
     *  @param p The player whose move to generate.
     *  @param x The x location of the peice.
     *  @param y The y location of the stone.
     *  @return A move_t that represent the move
     *              by player p at square (x, y).
     */
    move_t _generateMove(Player p, int x, int y);
    
    /**
     *  @param connected A set of tuple of two integers representing
     *                      the locations of connected stones.
     *                      Can be computed by _getConnected(x, y).
     *  @return A set of tuples of two integers representing
     *              the liberties of the connected stones.
     */
    std::set<std::tuple<int, int>> _getLiberties(std::set<std::tuple<int, int>> connected);
    
    /**
     *  @param x The x location of the stone.
     *  @param y The y location of the stone.
     *  @return A set of tuples of two integers representing
     *              the locations of all stones connected to 
     *              the stone at square (x, y).
     */
    std::set<std::tuple<int, int>> _getConnected(int x, int y);
    
public:
    
    Board(int _width = 19, int _height = 19, bool _prohibitSuicide = true);
    
    ~Board();
    
    /**
     *  Initializes the board to start the game.
     */
    void clear();
    
    /**
     *  @param p The player making the move.
     *  @param x The x location of the move.
     *  @param y The y location of the move.
     *  @return True if the move was successfully made.
     */
    bool move(Player p, int x, int y);
    
    /**
     *  Takes back the previous move and preserves history.
     *  @return True if the move was undone.
     */
    bool undo();
    
    /**
     *  Redoes the next move and preserves history.
     *  @return True if the next move was redone.
     */
    bool redo();
    
    /**
     *  Player p passes instead of makes a move.
     *  If the opponent also passes, player p loses.
     *  @param p The player to pass.
     */
    void pass(Player p);
    
    /**
     *
     */
    std::tuple<int, int> getScore();
};


#endif /* Board_hpp */
