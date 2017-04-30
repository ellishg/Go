//
//  Board.cpp
//  Go
//
//  Created by Ellis Hoag on 4/5/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Board.hpp"
#include <iostream>

using namespace std;

Player opponentPlayer(Player p) {
    if (p != NEITHER) {
        return (p == WHITE) ? BLACK : WHITE;
    }
    return NEITHER;
}

Board::Board(int _size, bool _prohibitSuicide) {
    
    size = _size;
    prohibitSuicide = _prohibitSuicide;
    
    board = new Player[size * size];
    
    clear();
}

Board::~Board() {
    
    delete [] board;
}

Player Board::_getStone(int x, int y) {
    if (_isOnBoard(x, y)) {
        return board[x + size * y];
    }
    return NEITHER;
}

set<tuple<Player, int, int>> Board::getStones()  {
    
    set<tuple<Player, int, int>> stones;
    
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            
            Player owner = _getStone(x, y);
            
            if (owner != NEITHER) {
                stones.insert(make_tuple(owner, x + 1, y + 1));
            }
        }
    }
    
    return stones;
}

Board & Board::operator=(const Board & other) {
    
    if (this != &other) {
        
        size = other.size;
        prohibitSuicide = other.prohibitSuicide;
        history = other.history;
        nextToMove = other.nextToMove;
        didJustPass = other.didJustPass;
        winner = other.winner;
        
        delete [] board;
        board = new Player[size * size];
        
        for (size_t i = 0; i < size * size; i++) {
            board[i] = other.board[i];
        }
    }
    
    return *this;
}

void Board::clear() {
    
    didJustPass = false;
    
    winner = NEITHER;
    
    _playerToMove = WHITE;
    
    history.clear();
    
    _clearBoard();
}

bool Board::move(int x, int y) {
    
    move_t move = _generateMove(_playerToMove, x - 1, y - 1);

    if (move.isValid()) {
        
        history.pushMove(move);
        _applyMove(move);
        
        didJustPass = false;
        _playerToMove = opponentPlayer(_playerToMove);
        return true;
    }
    
    return false;
}

void Board::_applyMove(move_t move) {
    
    int x = get<0>(move.placedStone);
    int y = get<1>(move.placedStone);
    
    _setStone(move.mover, x, y);
    
    for (auto killed : move.killedStones) {
        _clearStone(get<0>(killed), get<1>(killed));
    }
}


bool Board::undo() {
    
    move_t antiMove = history.popMove();
    
    if (!antiMove.isValid()) {
        return false;
    }
    
    _repealMove(antiMove);
    _playerToMove = opponentPlayer(_playerToMove);
    
    return true;
}

bool Board::redo() {
    
    move_t move = history.nextMove();
    
    if (!move.isValid()) {
        return false;
    }
    
    _applyMove(move);
    _playerToMove = opponentPlayer(_playerToMove);

    return true;
}

void Board::pass() {
    if (winner == NEITHER && didJustPass) {
        winner = _playerToMove;
    }
    didJustPass = true;
    _playerToMove = opponentPlayer(_playerToMove);
}

set<tuple<int, int>> Board::_getAdjacent(Player p, int x, int y) {
    
    set<tuple<int, int>> neighbors;
    
    const int locs[8] = {
        x    , y - 1,
        x + 1, y,
        x    , y + 1,
        x - 1, y
    };
    
    for (size_t i = 0; i < 4; i++) {
        int u = locs[2 * i + 0];
        int v = locs[2 * i + 1];
        if (_isOnBoard(u, v) && _getStoneOwner(u, v) == p) {
            neighbors.insert(tuple<int, int>(u, v));
        }
    }
    
    return neighbors;
}

/**
 *  In a given position, a liberty of a stone is an empty
 *  intersection adjacent to that stone or adjacent to a
 *  stone which is connected to that stone.
 */
set<tuple<int, int>> Board::_getLiberties(set<tuple<int, int>> connected) {
    
    set<tuple<int, int>> liberties;
    
    for (auto & stone : connected) {
        
        auto neighbors = _getAdjacent(NEITHER, get<0>(stone), get<1>(stone));
        
        liberties.insert(neighbors.begin(), neighbors.end());
    }
    
    return liberties;
}

tuple<set<tuple<int, int>>, set<tuple<int, int>>> Board::getLiberties() {
    
    set<tuple<int, int>> whiteSet;
    set<tuple<int, int>> blackSet;
    
    set<tuple<int, int>> connected;
    set<tuple<int, int>> liberties;
    
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            switch(_getStone(x, y)) {
                case WHITE:
                    connected = _getConnected(x, y);
                    liberties = _getLiberties(connected);
                    whiteSet.insert(liberties.begin(), liberties.end());
                    break;
                case BLACK:
                    connected = _getConnected(x, y);
                    liberties = _getLiberties(connected);
                    blackSet.insert(liberties.begin(), liberties.end());
                    break;
                case NEITHER:
                    break;
            }
        }
    }
    
    return make_tuple(whiteSet, blackSet);
}


/**
 *  Two placed stones of the same color (or two empty intersections)
 *  are said to be connected if it is possible to draw a path from
 *  one intersection to the other by passing through adjacent
 *  intersections of the same state (empty, occupied by white, or
 *  occupied by black).
 */
set<tuple<int, int>> Board::_getConnected(int x, int y) {
    
    Player owner = _getStoneOwner(x, y);
    
    if (!_isOnBoard(x, y) || owner == NEITHER) {
        return set<tuple<int, int>>();
    }
    
    set<tuple<int, int>> connected;
    
    queue<tuple<int, int>> bag;

    bag.push(make_tuple(x, y));

    while (!bag.empty()) {
        auto stone = bag.front();
        bag.pop();
        
        if (connected.find(stone) == connected.end()) {
            connected.insert(stone);
            
            auto neighbors = _getAdjacent(owner, get<0>(stone), get<1>(stone));
            
            for (auto & neighbor : neighbors) {
                bag.push(neighbor);
            }
        }
    }
    
    return connected;
}

tuple<int, int> Board::getScore() {
    
    int blackScore = 0;
    int whiteScore = 0;
    
    for (size_t i = 0; i < size * size; i++) {
        if (board[i] == WHITE) {
            whiteScore++;
        }
        else if (board[i] == BLACK) {
            blackScore++;
        }
    }
    
    return make_tuple(whiteScore, blackScore);
}

void Board::_clearBoard() {
    for (int i = 0; i < size * size; i++) {
        board[i] = NEITHER;
    }
}

void Board::_setStone(Player p, int x, int y) {
    if (_isOnBoard(x, y)) {
        board[x + size * y] = p;
    }
}

Player Board::_getStoneOwner(int x, int y) {
    if (!_isOnBoard(x, y)) {
        return NEITHER;
    }
    return board[x + size * y];
}

bool Board::_isOnBoard(int x, int y) {
    
    return (0 <= x && x < size &&
            0 <= y && y < size);
}

void Board::_clearStone(int x, int y) {
    if (_isOnBoard(x, y)) {
        board[x + size * y] = NEITHER;
    }
}

void Board::_repealMove(move_t antiMove) {
    
    int x = get<0>(antiMove.placedStone);
    int y = get<1>(antiMove.placedStone);
    
    _clearStone(x, y);
    
    Player opponent = opponentPlayer(antiMove.mover);
    
    for (auto recovered : antiMove.killedStones) {
        _setStone(opponent, get<0>(recovered), get<1>(recovered));
    }
}

/**
 *  Step 1: Removing from the board any stones of their opponent's
 *          color that have no liberties.
 *  Step 2: Removing from the board any stones of their own color
 *          that have no liberties.
 *  Step 3: (Optional) Prohibition of suicide: A play is illegal
 *          if one or more stones of that player's color would be
 *          removed in Step 2 of that play.
 *  TODO: Not yet implemented.
 *  Step 4: A play is illegal if it would have the effect (after
 *          all steps of the play have been completed) of creating
 *          a position that has occurred previously in the game.
 *
 */
move_t Board::_generateMove(Player p, int x, int y) {
    
    if (winner == NEITHER && _isOnBoard(x, y) && p != NEITHER) {
        if (_getStoneOwner(x, y) == NEITHER) {
            
            move_t move;
            move.mover = p;
            move.placedStone = make_tuple(x, y);
            
            Player opponent = opponentPlayer(p);
            
            _setStone(p, x, y);
            
            auto neighboringFoes = _getAdjacent(opponent, x, y);
            auto neighboringFriends = _getAdjacent(p, x, y);
            neighboringFriends.insert(make_tuple(x, y));
            
            for (auto & stone : neighboringFoes) {
                auto connected = _getConnected(get<0>(stone), get<1>(stone));
                auto liberties = _getLiberties(connected);
                
                if (liberties.empty()) {
                    for (auto & killed : connected) {
                        move.killedStones.push_back(killed);
                        _clearStone(get<0>(killed), get<1>(killed));
                    }
                }
            }
            
            for (auto & stone : neighboringFriends) {
                auto connected = _getConnected(get<0>(stone), get<1>(stone));
                auto liberties = _getLiberties(connected);
                
                if (liberties.empty()) {
                    if (prohibitSuicide) {
                        
                        for (auto & killed : move.killedStones) {
                            _setStone(opponent, get<0>(killed), get<1>(killed));
                        }
                        _clearStone(x, y);
                        
                        return move_t();
                    }
                    else {
                        for (auto & killed : connected) {
                            move.killedStones.push_back(killed);
                        }
                    }
                }
            }
            
            
            for (auto & killed : move.killedStones) {
                _setStone(opponent, get<0>(killed), get<1>(killed));
            }
            _clearStone(x, y);
            
            return move;
        }
    }
    
    return move_t();
}

/**
 *  (
 *  ;FF[4]
 *  GM[1]
 *  CA[iso-8859-1]
 *  AP[EllisGo]
 *  US[Ellis Hoag]
 *  DT[TODAY'S DATE]
 *  SZ[19]
 *  HA[0]
 *  KM[0.0]
 *  ;B[aa];W[df];B[cd];...)
 */
char * Board::_serialize() {
    
    time_t rawTime;
    time(&rawTime);
    struct tm * timeInfo = localtime(&rawTime);
    
    int year = timeInfo->tm_year + 1900;
    int month = timeInfo->tm_mon + 1;
    int day = timeInfo->tm_mday;
    
    const size_t headerLength = 94;
    const size_t footerLength = 2;
    char header[headerLength + 1];
    snprintf(header, headerLength + 1,
        "(\n"
        ";FF[4]\n"
        "GM[1]\n"
        "CA[iso-8859-1]\n"
        "AP[EllisGo]\n"
        "US[Ellis Hoag]\n"
        "DT[%04d-%02d-%02d]\n"
        "SZ[%02d]\n"
        "HA[0]\n"
        "KM[0.0]\n"
        ";", year, month, day, size);
    const char footer[footerLength + 1] = ")\n";
    
    vector<move_t> moves = history.getMoves();
    
    size_t movesLength = moves.size() * 6;
    size_t dataLength = movesLength + headerLength + footerLength;
    char * data = (char *)malloc(dataLength + 1);
    if (!data) {
        return NULL;
    }
    data[dataLength] = '\0';
    
    sprintf(data, "%s", header);

    size_t i = headerLength;
    for (move_t move : moves) {
    
        char player = (move.mover == WHITE) ? 'W' : 'B';
        char x = 'a' + get<0>(move.placedStone);
        char y = 'a' + get<1>(move.placedStone);
        
        sprintf(data + i, "%c[%c%c];", player, x, y);
        
        i += 6;
    }
    
    sprintf(data + i, "%s", footer);
    
    return data;
}

/**
 *  (
 *  ;FF[4]
 *  GM[1]
 *  CA[iso-8859-1]
 *  AP[EllisGo]
 *  US[Ellis Hoag]
 *  DT[TODAY'S DATE]
 *  SZ[19]
 *  HA[0]
 *  KM[0.0]
 *  ;B[aa];W[df];B[cd];...)
 */

bool Board::_deserialize(char * data, size_t length) {
    
    vector<tuple<int, int>> moveList;

    const char * token = strtok(data, ";");
    while (token) {
        
        if (token[1] == '[' && token[4] == ']') {
            if (token[0] == 'B' || token[0] == 'W') {
                int x = (int)(token[2] - 'a') + 1;
                int y = (int)(token[3] - 'a') + 1;
                moveList.push_back(make_tuple(x, y));
            }
        }
        
        token = strtok(NULL, ";");
    }
    
    Board newBoard(size, prohibitSuicide);
    
    for (tuple<int, int> move : moveList) {
        if (!newBoard.move(get<0>(move), get<1>(move))) {
            return false;
        }
    }
    
    *this = newBoard;
    return true;
}

bool Board::exportSGF(const char * dir) {
    
    char * data = _serialize();
    
    if (data) {
        
        FILE * file = fopen(dir, "w");
        
        if (!file) {
            delete data;
            perror("Error opening file");
            return false;
        }
        
        fprintf(file, "%s", data);
        
        fclose(file);
        free(data);
        
        return true;
    }
    
    return false;
}

bool Board::importSGF(const char * dir) {
    
    FILE * file = fopen(dir, "r");
    
    if (!file) {
        perror("Error opening file");
        return false;
    }
    
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char * data = (char *)malloc(length + 1);
    data[length + 1] = '\0';
    
    fread((void *)data, 1, length, file);
    
    bool result = _deserialize(data, length);
    
    free(data);
    
    return result;
}

Player Board::hasWon() {
    return winner;
}
