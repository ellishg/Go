//
//  main.cpp
//  GoGUI
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "SDL2/SDL.h"
#include <set>
#include <tuple>
#include <stdio.h>

#include "Board.hpp"
#include "Window.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Button.hpp"
#include "Text.hpp"

using namespace std;

const int windowWidth = 565;
const int windowHeight = 645;
int boardNumSquares = 19;
const int boardSize = 420;
const int boardX = (windowWidth - boardSize) / 2;
const int boardY = 70;
int stoneSize = boardSize / boardNumSquares;
const int maxStonesPerColor = 19 * 19 / 2;

bool showLiberties = false;
bool playComputer = false;

size_t breadth;
size_t depth;

Sprite * board;
Texture * boardTexture19;
Texture * boardTexture13;
Texture * boardTexture9;
Text * alert;
Text * score;
Text * info;
Text * thinking;
Window * myWindow;
Board * myBoard;
Sprite * whitePieces[maxStonesPerColor];
Sprite * blackPieces[maxStonesPerColor];
Sprite * whiteLiberties[maxStonesPerColor];
Sprite * blackLiberties[maxStonesPerColor];

void importSGF() {
    const char * filename = "/Users/ellis/Desktop/game.sgf";
    myBoard->importSGF(filename);
}

void exportSGF() {
    const char * filename = "/Users/ellis/Desktop/game.sgf";
    myBoard->exportSGF(filename);
}

void newGame() {
    myBoard->clear();
}

void updateBoardSize() {
    stoneSize = boardSize / boardNumSquares;
    for (size_t i = 0; i < maxStonesPerColor; i++) {
        whitePieces[i]->width = stoneSize;
        whitePieces[i]->height = stoneSize;
        blackPieces[i]->width = stoneSize;
        blackPieces[i]->height = stoneSize;
        whiteLiberties[i]->width = stoneSize;
        whiteLiberties[i]->height = stoneSize;
        blackLiberties[i]->width = stoneSize;
        blackLiberties[i]->height = stoneSize;
    }
    delete myBoard;
    myBoard = new Board(boardNumSquares);

}

void makeBoard19() {
    board->_texture = boardTexture19;
    boardNumSquares = 19;
    updateBoardSize();
}

void makeBoard13() {
    board->_texture = boardTexture13;
    boardNumSquares = 13;
    updateBoardSize();
}

void makeBoard9() {
    board->_texture = boardTexture9;
    boardNumSquares = 9;
    updateBoardSize();
}

void vsHuman() {
    newGame();
    playComputer = false;
}

void vsComputerNovice() {
    newGame();
    playComputer = true;
    breadth = 200;
    depth = 7;
}

void vsComputerExpert() {
    newGame();
    playComputer = true;
    breadth = 500;
    depth = 7;
}

void undo() {
    myBoard->undo();
}

void redo() {
    myBoard->redo();
}

void pass() {
    myBoard->pass();
}

void quit() {
    myWindow->quit();
}

void toggleLiberties() {
    showLiberties = !showLiberties;
}

tuple<int, int> textureToBoard(int u, int v) {
    
    int x = ((u - boardX - stoneSize / 2.f) / (boardSize + stoneSize)) * boardNumSquares + 2;
    int y = ((v - boardY - stoneSize / 2.f) / (boardSize + stoneSize)) * boardNumSquares + 2;
    
    return make_tuple(x, y);
}

tuple<int, int> boardToTexture(int x, int y) {
    
    int u = (x - 2.f) / boardNumSquares * (boardSize + stoneSize) + boardX + stoneSize / 2.f + 2;
    int v = (y - 2.f) / boardNumSquares * (boardSize + stoneSize) + boardY + stoneSize / 2.f + 2;
    
    return make_tuple(u, v);
}

void onMouseClick(int x, int y) {
    
    if (boardX - stoneSize / 2 < x && x < boardX + boardSize + stoneSize / 2
        && boardY - stoneSize / 2 < y && y < boardY + boardSize + stoneSize / 2) {
        
        tuple<int, int> loc = textureToBoard(x, y);
        
        myBoard->move(get<0>(loc), get<1>(loc));
    }
}

void onLoop() {
    
    static Player oldPlayer = WHITE;
    if (oldPlayer != myBoard->playerToMove()) { // Hack to render stone before computer starts to think.
        if (playComputer && oldPlayer == WHITE) {
            thinking->setText("Thinking...");
        }
        oldPlayer = myBoard->playerToMove();
    }
    else if (playComputer && myBoard->playerToMove() == BLACK) {
        myBoard->applyBestMove(breadth, depth);
        thinking->setText("");
    }
    
    set<tuple<Player, int, int>> stones = myBoard->getStones();
    
    size_t iWhiteStone = 0;
    size_t iBlackStone = 0;
    
    for (auto stone : stones) {
        
        int x = get<1>(stone);
        int y = get<2>(stone);
        tuple<int, int> loc = boardToTexture(x, y);
        
        switch (get<0>(stone)) {
            case WHITE:
                whitePieces[iWhiteStone]->x = get<0>(loc);
                whitePieces[iWhiteStone]->y = get<1>(loc);
                iWhiteStone++;
                break;
            case BLACK:
                blackPieces[iBlackStone]->x = get<0>(loc);
                blackPieces[iBlackStone]->y = get<1>(loc);
                iBlackStone++;
                break;
            default:
                break;
        }
    }
    
    for (; iWhiteStone < maxStonesPerColor; iWhiteStone++) {
        whitePieces[iWhiteStone]->x = -100;
        whitePieces[iWhiteStone]->y = -100;
    }
    
    for (; iBlackStone < maxStonesPerColor; iBlackStone++) {
        blackPieces[iBlackStone]->x = -100;
        blackPieces[iBlackStone]->y = -100;
    }
    
    if (showLiberties) {
        size_t iWhiteLiberty = 0;
        size_t iBlackLiberty = 0;
        
        tuple<set<tuple<int, int>>, set<tuple<int, int>>> liberties = myBoard->getLiberties();
        
        for (tuple<int, int> liberty : get<0>(liberties)) {
            int x = get<0>(liberty) + 1;
            int y = get<1>(liberty) + 1;
            tuple<int, int> loc = boardToTexture(x, y);
            
            whiteLiberties[iWhiteLiberty]->x = get<0>(loc);
            whiteLiberties[iWhiteLiberty]->y = get<1>(loc);
            iWhiteLiberty++;
        }
        
        for (tuple<int, int> liberty : get<1>(liberties)) {
            int x = get<0>(liberty) + 1;
            int y = get<1>(liberty) + 1;
            tuple<int, int> loc = boardToTexture(x, y);
            
            blackLiberties[iBlackLiberty]->x = get<0>(loc);
            blackLiberties[iBlackLiberty]->y = get<1>(loc);
            iBlackLiberty++;
        }
        
        for (; iWhiteLiberty < maxStonesPerColor; iWhiteLiberty++) {
            whiteLiberties[iWhiteLiberty]->x = -100;
            whiteLiberties[iWhiteLiberty]->y = -100;
        }
        
        for (; iBlackLiberty < maxStonesPerColor; iBlackLiberty++) {
            blackLiberties[iBlackLiberty]->x = -100;
            blackLiberties[iBlackLiberty]->y = -100;
        }
    }
    else {
        for (size_t i = 0; i < maxStonesPerColor; i++) {
            whiteLiberties[i]->x = -100;
            whiteLiberties[i]->y = -100;
            blackLiberties[i]->x = -100;
            blackLiberties[i]->y = -100;
        }
    }
    
    tuple<int, int> s = myBoard->getScore();
    char string[30];
    snprintf(string, 30, "Green: %d Red: %d", get<0>(s), get<1>(s));
    score->setText(string);
    
    switch (myBoard->playerToMove()) {
        case WHITE:
            alert->setText("Green to move.");
            break;
        case BLACK:
            alert->setText("Red to move.");
            break;
        default:
            break;
    }
    
    Player winner = myBoard->hasWon();
    if (winner != NEITHER) {
        switch (winner) {
            case WHITE:
                alert->setText("Green has won!");
                break;
            case BLACK:
                alert->setText("Red has won!");
                break;
            default:
                break;
        }
    }
    
    if (playComputer) {
        info->setText("Against Computer");
    }
    else {
        info->setText("Against Human");
    }
}

int main(int argc, const char * argv[]) {

    myBoard = new Board(boardNumSquares, true);
    
    myWindow = new Window(windowWidth, windowHeight);
    myWindow->setMouseClick(onMouseClick);
    myWindow->setOnLoop(onLoop);
    
    SDL_Renderer * renderer = myWindow->getRenderer();
    
    boardTexture19 = new Texture("/Users/ellis/GitHub/Go/assets/go-board-19x19.png", renderer);
    boardTexture13 = new Texture("/Users/ellis/GitHub/Go/assets/go-board-13x13.png", renderer);
    boardTexture9 = new Texture("/Users/ellis/GitHub/Go/assets/go-board-9x9.png", renderer);
    Texture * quitTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_quit.png", renderer);
    Texture * passTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_pass.png", renderer);
    Texture * undoTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_undo.png", renderer);
    Texture * redoTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_redo.png", renderer);
    Texture * toggleLibertiesTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_toggle-liberties.png", renderer);
    Texture * newGameTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_new-game.png", renderer);
    Texture * button19Texture = new Texture("/Users/ellis/GitHub/Go/assets/button_19x19.png", renderer);
    Texture * button13Texture = new Texture("/Users/ellis/GitHub/Go/assets/button_13x13.png", renderer);
    Texture * button9Texture = new Texture("/Users/ellis/GitHub/Go/assets/button_9x9.png", renderer);
    Texture * importTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_import.png", renderer);
    Texture * exportTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_export.png", renderer);
    Texture * vsHumanTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_vs-human.png", renderer);
    Texture * noviceTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_novice.png", renderer);
    Texture * expertTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_expert.png", renderer);
    Texture * whitePiecetexture = new Texture("/Users/ellis/GitHub/Go/assets/green_stone.png", renderer);
    Texture * blackPiecetexture = new Texture("/Users/ellis/GitHub/Go/assets/red_stone.png", renderer);
    Texture * whiteLibertytexture = new Texture("/Users/ellis/GitHub/Go/assets/green_liberty.png", renderer);
    Texture * blackLibertytexture = new Texture("/Users/ellis/GitHub/Go/assets/red_liberty.png", renderer);
    
    TTF_Font * font = TTF_OpenFont("/Users/ellis/GitHub/Go/assets/AppleGaramond.ttf", 24);

    board = new Sprite(boardTexture19, {{0, 0, 2400, 2400}});
    board->x = boardX;
    board->y = boardY;
    board->width = boardSize;
    board->height = boardSize;
    myWindow->addSprite(board);
    
    for (size_t i = 0; i < maxStonesPerColor; i++) {
        whitePieces[i] = new Sprite(whitePiecetexture, {{0, 0, 100, 100}});
        blackPieces[i] = new Sprite(blackPiecetexture, {{0, 0, 100, 100}});
        whiteLiberties[i] = new Sprite(whiteLibertytexture, {{0, 0, 100, 100}});
        blackLiberties[i] = new Sprite(blackLibertytexture, {{0, 0, 100, 100}});
        
        whitePieces[i]->x = -100;
        whitePieces[i]->y = -100;
        whitePieces[i]->width = stoneSize;
        whitePieces[i]->height = stoneSize;
        blackPieces[i]->x = -100;
        blackPieces[i]->y = -100;
        blackPieces[i]->width = stoneSize;
        blackPieces[i]->height = stoneSize;
        whiteLiberties[i]->x = -100;
        whiteLiberties[i]->y = -100;
        whiteLiberties[i]->width = stoneSize;
        whiteLiberties[i]->height = stoneSize;
        blackLiberties[i]->x = -100;
        blackLiberties[i]->y = -100;
        blackLiberties[i]->width = stoneSize;
        blackLiberties[i]->height = stoneSize;
        
        myWindow->addSprite(whitePieces[i]);
        myWindow->addSprite(blackPieces[i]);
        myWindow->addSprite(whiteLiberties[i]);
        myWindow->addSprite(blackLiberties[i]);
        
    }
    
    alert = new Text(renderer, 5, 5, font);
    myWindow->addText(alert);
    
    score = new Text(renderer, 200, 5, font);
    myWindow->addText(score);
    
    info = new Text(renderer, 400, 5, font);
    myWindow->addText(info);
    
    thinking = new Text(renderer, 400, 35, font);
    thinking->setText("");
    myWindow->addText(thinking);
    
    Button quitButton({5, 600, 82, 40}, quitTexture, {0, 0, 82, 40}, {0, 40, 82, 40}, quit);
    myWindow->addButton(quitButton);
    
    Button passButton({92, 600, 80, 40}, passTexture, {0, 0, 80, 40}, {0, 40, 80, 40}, pass);
    myWindow->addButton(passButton);
    
    Button undoButton({177, 600, 91, 40}, undoTexture, {0, 0, 91, 40}, {0, 40, 91, 40}, undo);
    myWindow->addButton(undoButton);
    
    Button redoButton({273, 600, 88, 40}, redoTexture, {0, 0, 88, 40}, {0, 40, 88, 40}, redo);
    myWindow->addButton(redoButton);
    
    Button toggleLibertiesButton({366, 600, 194, 40}, toggleLibertiesTexture, {0, 0, 194, 40}, {0, 40, 194, 40}, toggleLiberties);
    myWindow->addButton(toggleLibertiesButton);
    
    Button newGameButton({5, 510, 145, 40}, newGameTexture, {0, 0, 145, 40}, {0, 40, 145, 40}, newGame);
    myWindow->addButton(newGameButton);
    
    Button new19Button({5, 555, 95, 40}, button19Texture, {0, 0, 95, 40}, {0, 40, 95, 40}, makeBoard19);
    myWindow->addButton(new19Button);

    Button new13Button({105, 555, 95, 40}, button13Texture, {0, 0, 95, 40}, {0, 40, 95, 40}, makeBoard13);
    myWindow->addButton(new13Button);
    
    Button new9Button({205, 555, 73, 40}, button9Texture, {0, 0, 73, 40}, {0, 40, 73, 40}, makeBoard9);
    myWindow->addButton(new9Button);
    
    Button importButton({155, 510, 105, 40}, importTexture, {0, 0, 105, 40}, {0, 40, 105, 40}, importSGF);
    myWindow->addButton(importButton);
    
    Button exportButton({265, 510, 102, 40}, exportTexture, {0, 0, 102, 40}, {0, 40, 102, 40}, exportSGF);
    myWindow->addButton(exportButton);
    
    Button vsHumanButton({372, 510, 135, 40}, vsHumanTexture, {0, 0, 135, 40}, {0, 40, 135, 40}, vsHuman);
    myWindow->addButton(vsHumanButton);
    
    Button noviceButton({283, 555, 107, 40}, noviceTexture, {0, 0, 107, 40}, {0, 40, 107, 40}, vsComputerNovice);
    myWindow->addButton(noviceButton);

    Button expertButton({395, 555, 104, 40}, expertTexture, {0, 0, 104, 40}, {0, 40, 104, 40}, vsComputerExpert);
    myWindow->addButton(expertButton);

    myWindow->launchWindow();
    
    TTF_CloseFont(font);
    delete alert;
    delete score;
    delete info;
    delete thinking;
    delete board;
    delete boardTexture19;
    delete boardTexture13;
    delete boardTexture9;
    delete quitTexture;
    delete undoTexture;
    delete redoTexture;
    delete newGameTexture;
    delete button19Texture;
    delete button13Texture;
    delete button9Texture;
    delete toggleLibertiesTexture;
    delete importTexture;
    delete exportTexture;
    delete vsHumanTexture;
    delete noviceTexture;
    delete expertTexture;
    delete whitePiecetexture;
    delete blackPiecetexture;
    delete whiteLibertytexture;
    delete blackLibertytexture;
    delete myWindow;
    delete myBoard;
    
    for (size_t i = 0; i < maxStonesPerColor; i++) {
        delete whitePieces[i];
        delete blackPieces[i];
    }
    
    return 0;
}
