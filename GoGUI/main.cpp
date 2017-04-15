//
//  main.cpp
//  GoGUI
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "SDL2/SDL.h"

#include "Board.hpp"
#include "Window.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Button.hpp"
#include "Text.hpp"

using namespace std;

Text * score;
Window * myWindow;

int boardX = 20;
int boardY = 50;
int boardSize = 460;

void addScore() {
    score->setText("hi");
    cout << "hi\n";
}

void quit() {
    myWindow->quit();
}

void onMouseClick(int x, int y) {
    
    if (boardX < x && x < boardX + boardSize
        && boardY < y && y < boardY + boardSize) {
        
    }
}

int main(int argc, const char * argv[]) {

    myWindow = new Window(500, 600);
    myWindow->setMouseClick(onMouseClick);
    
    SDL_Renderer * renderer = myWindow->getRenderer();
    
    Texture * boardTexture = new Texture("/Users/ellis/GitHub/Go/assets/go-board-19x19.png", renderer);
    Texture * quitTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_quit.png", renderer);
    Texture * passTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_pass.png", renderer);
    Texture * toggleLibertiesTexture = new Texture("/Users/ellis/GitHub/Go/assets/button_toggle-liberties.png", renderer);
    Texture * whitePiecetexture = new Texture("/Users/ellis/GitHub/Go/assets/white_piece.png", renderer);
    Texture * blackPiecetexture = new Texture("/Users/ellis/GitHub/Go/assets/black_piece.png", renderer);
    
    TTF_Font * font = TTF_OpenFont("/Users/ellis/GitHub/Go/assets/AppleGaramond.ttf", 16);

    Sprite * board = new Sprite(boardTexture, {{0, 0, 2400, 2400}});
    board->x = boardX;
    board->y = boardY;
    board->width = boardSize;
    board->height = boardSize;
    myWindow->addSprite(board);
    
    Sprite * piece = new Sprite(blackPiecetexture, {{0, 0, 100, 100}});
    piece->x = 10;
    piece->y = 40;
    piece->width = 20;
    piece->height = 20;
    myWindow->addSprite(piece);
    
    Sprite * piece1 = new Sprite(whitePiecetexture, {{0, 0, 100, 100}});
    piece1->x = 35;
    piece1->y = 40;
    piece1->width = 20;
    piece1->height = 20;
    myWindow->addSprite(piece1);
    
    score = new Text(renderer, 5, 5, font);
    myWindow->addText(score);
    
    Button quitButton({5, 555, 82, 40}, quitTexture, {0, 0, 82, 40}, {0, 40, 82, 40}, quit);
    myWindow->addButton(quitButton);
    
    Button passButton({92, 555, 80, 40}, passTexture, {0, 0, 80, 40}, {0, 40, 80, 40}, addScore);
    myWindow->addButton(passButton);
    
    Button toggleLibertiesButton({177, 555, 194, 40}, toggleLibertiesTexture, {0, 0, 194, 40}, {0, 40, 194, 40}, quit);
    myWindow->addButton(toggleLibertiesButton);
    
    myWindow->launchWindow();
    
    TTF_CloseFont(font);
    delete score;
    delete board;
    delete boardTexture;
    delete quitTexture;
    delete myWindow;
    
    return 0;
}
