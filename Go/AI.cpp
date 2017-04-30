//
//  AI.cpp
//  Go
//
//  Created by Ellis Hoag on 4/29/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Board.hpp"

using namespace std;

/**
 *  Assume that the computer is black.
 */
void Board::applyBestMove(size_t breadth, size_t depth) {
    
    vector<move_t> allMoves = _allValidMoves();
    size_t numFirstMoves = allMoves.size();
    
    size_t bestMoveIdx = 0;
    float bestAvgScore = 0;
    
    Board testBoard;
    for (size_t i = 0; i < numFirstMoves; i++) {
                
        int totalScore = 0;
        
        for (size_t j = 0; j < breadth; j++) {
            
            testBoard = *this;
            testBoard._applyMove(allMoves[i]);
            testBoard._simulateGame(depth);
            tuple<int, int> score = testBoard.getScore();
            totalScore += get<1>(score) - get<0>(score); // Assume the computer is black.
        }
        
        float avgScore = (float)totalScore / breadth;
        if (avgScore > bestAvgScore) {
            bestAvgScore = avgScore;
            bestMoveIdx = i;
        }
    }
    
    int bestX = get<0>(allMoves[bestMoveIdx].placedStone);
    int bestY = get<1>(allMoves[bestMoveIdx].placedStone);
    
    move(bestX + 1, bestY + 1);

}

void Board::_simulateGame(size_t depth) {
    
    for (size_t i = 0; i < depth; i++) {
        
        move_t randomMove = _randomMove();
        
        if (randomMove.isValid()) {
            int x = get<0>(randomMove.placedStone);
            int y = get<1>(randomMove.placedStone);
            
            move(x + 1, y + 1);
        }
        else {
            return;
        }
    }
}

vector<move_t> Board::_allValidMoves() {
    
    vector<move_t> moves;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            
            move_t m = _generateMove(BLACK, i, j);
            
            if (m.isValid()) {
                moves.push_back(m);
            }
        }
    }
    
    return moves;
}

move_t Board::_randomMove() {

    move_t randomMove;
    
    while(!randomMove.isValid()) {
    
        int m = rand() % (size * size);
        int x = m / size;
        int y = m % size;
        
        randomMove = _generateMove(BLACK, x, y);
    }
    
    return randomMove;
    
    // Super slow runtime because c++ is not lazy.
    /*
    vector<move_t> moves = _allValidMoves();
    
    if (moves.empty()) {
        return move_t();
    }
    
    random_shuffle(moves.begin(), moves.end());
    
    return moves[0];
     */
}
