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
const int boardNumSquares = 19;
const int boardSize = 460;
const int boardX = (windowWidth - boardSize) / 2;
const int boardY = 40;
const int stoneSize = boardSize / boardNumSquares;
const int numStonesPerColor = boardNumSquares * boardNumSquares / 2;

bool showLiberties = false;
bool playComputer = true;

Text * alert;
Text * score;
Window * myWindow;
Board * myBoard;
Sprite * whitePieces[numStonesPerColor];
Sprite * blackPieces[numStonesPerColor];
Sprite * whiteLiberties[numStonesPerColor];
Sprite * blackLiberties[numStonesPerColor];

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

void vsHuman() {
    newGame();
    playComputer = false;
}

void vsComputer() {
    newGame();
    playComputer = true;
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
    
    for (; iWhiteStone < numStonesPerColor; iWhiteStone++) {
        whitePieces[iWhiteStone]->x = -100;
        whitePieces[iWhiteStone]->y = -100;
    }
    
    for (; iBlackStone < numStonesPerColor; iBlackStone++) {
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
        
        for (; iWhiteLiberty < numStonesPerColor; iWhiteLiberty++) {
            whiteLiberties[iWhiteLiberty]->x = -100;
            whiteLiberties[iWhiteLiberty]->y = -100;
        }
        
        for (; iBlackLiberty < numStonesPerColor; iBlackLiberty++) {
            blackLiberties[iBlackLiberty]->x = -100;
            blackLiberties[iBlackLiberty]->y = -100;
        }
    }
    else {
        for (size_t i = 0; i < numStonesPerColor; i++) {
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
}

int main(int argc, const char * argv[]) {

    myBoard = new Board(boardNumSquares, true);
    
    myWindow = new Window(windowWidth, windowHeight);
    myWindow->setMouseClick(onMouseClick);
    myWindow->setOnLoop(onLoop);
    
    SDL_Renderer * renderer = myWindow->getRenderer();
    
    Texture * boardTexture = new Texture("/Users/ellis/GitHub/Go/assets/go-board-19x19.png", renderer);
    Texture * quitTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_quit.png", renderer);
    Texture * passTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_pass.png", renderer);
    Texture * undoTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_undo.png", renderer);
    Texture * redoTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_redo.png", renderer);
    Texture * toggleLibertiesTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_toggle-liberties.png", renderer);
    Texture * newGameTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_new-game.png", renderer);
    Texture * importTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_import.png", renderer);
    Texture * exportTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_export.png", renderer);
    Texture * vsHumanTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_vs-human.png", renderer);
    Texture * vsComputerTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_vs-computer.png", renderer);
    Texture * whitePiecetexture = new Texture("/Users/ellis/GitHub/Go/assets/green_stone.png", renderer);
    Texture * blackPiecetexture = new Texture("/Users/ellis/GitHub/Go/assets/red_stone.png", renderer);
    Texture * whiteLibertytexture = new Texture("/Users/ellis/GitHub/Go/assets/green_liberty.png", renderer);
    Texture * blackLibertytexture = new Texture("/Users/ellis/GitHub/Go/assets/red_liberty.png", renderer);
    
    TTF_Font * font = TTF_OpenFont("/Users/ellis/GitHub/Go/assets/AppleGaramond.ttf", 24);

    Sprite * board = new Sprite(boardTexture, {{0, 0, 2400, 2400}});
    board->x = boardX;
    board->y = boardY;
    board->width = boardSize;
    board->height = boardSize;
    myWindow->addSprite(board);
    
    for (size_t i = 0; i < numStonesPerColor; i++) {
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
    
    Button importButton({155, 510, 105, 40}, importTexture, {0, 0, 105, 40}, {0, 40, 105, 40}, importSGF);
    myWindow->addButton(importButton);
    
    Button exportButton({265, 510, 102, 40}, exportTexture, {0, 0, 102, 40}, {0, 40, 102, 40}, exportSGF);
    myWindow->addButton(exportButton);
    
    Button vsHumanButton({372, 510, 135, 40}, vsHumanTexture, {0, 0, 135, 40}, {0, 40, 135, 40}, vsHuman);
    myWindow->addButton(vsHumanButton);
    
    Button vsComputerButton({372, 555, 164, 40}, vsComputerTexture, {0, 0, 164, 40}, {0, 40, 164, 40}, vsComputer);
    myWindow->addButton(vsComputerButton);
    
    myWindow->launchWindow();
    
    TTF_CloseFont(font);
    delete alert;
    delete score;
    delete board;
    delete boardTexture;
    delete quitTexture;
    delete undoTexture;
    delete redoTexture;
    delete newGameTexture;
    delete toggleLibertiesTexture;
    delete importTexture;
    delete exportTexture;
    delete vsHumanTexture;
    delete vsComputerTexture;
    delete whitePiecetexture;
    delete blackPiecetexture;
    delete whiteLibertytexture;
    delete blackLibertytexture;
    delete myWindow;
    delete myBoard;
    
    for (size_t i = 0; i < numStonesPerColor; i++) {
        delete whitePieces[i];
        delete blackPieces[i];
    }
    
    return 0;
}
