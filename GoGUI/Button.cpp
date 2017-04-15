//
//  Button.cpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Button.hpp"

using namespace std;

Button::Button(int x, int y, Texture * sprite, SDL_Rect clipUp, SDL_Rect clipDown, function<void()> pressed) {
    
    _x = x;
    _y = y;
    
    _sprite = sprite;
    _clipUp = clipUp;
    _clipDown = clipDown;
    _justPressed = false;
    
    _pressed = pressed;
    
    _state = BUTTON_UP;
}

void Button::render() {
    
    SDL_Rect clip;
    
    switch (_state) {
        case BUTTON_UP:
            clip = _clipUp;
            break;
        case BUTTON_DOWN:
            clip = _clipDown;
            break;
    }
    
    _sprite->render(_x, _y, &clip);
}

void Button::handleEvent(SDL_Event * event) {
    
    if(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        if (_x < mouseX && mouseX < _sprite->getWidth()
            && _y < mouseY && mouseY < _sprite->getHeight()) {
            
            switch (event->type) {
                case SDL_MOUSEBUTTONDOWN:
                    _state = BUTTON_DOWN;
                    if (!_justPressed) {
                        _justPressed = true;
                        _pressed();
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    _state = BUTTON_UP;
                    _justPressed = false;
                    break;
                default:
                    break;
            }
            
        }
    }

}
