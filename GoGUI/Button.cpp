//
//  Button.cpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Button.hpp"

using namespace std;

Button::Button(SDL_Rect tgt, Texture * texture, SDL_Rect clipUp, SDL_Rect clipDown, function<void()> pressed) : _sprite(texture, {clipUp, clipDown}) {
    
    _sprite.x = tgt.x;
    _sprite.y = tgt.y;
    _sprite.width = tgt.w;
    _sprite.height = tgt.h;
    _sprite.state = BUTTON_UP;
        
    _pressed = pressed;
}

void Button::render() {
    
    _sprite.render();
}

void Button::handleEvent(SDL_Event * event) {
    
    if(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        if (_sprite.x < mouseX && mouseX < _sprite.x + _sprite.width
            && _sprite.y < mouseY && mouseY < _sprite.y + _sprite.height) {
            
            switch (event->type) {
                case SDL_MOUSEBUTTONDOWN:
                    if (_sprite.state == BUTTON_UP) {
                        _sprite.state = BUTTON_DOWN;
                        _pressed();
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    _sprite.state = BUTTON_UP;
                    break;
                default:
                    break;
            }
            
        }
    }

}
