//
//  Button.hpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include "SDL2/SDL.h"
#include <functional>

#include "Texture.hpp"

enum State {
    BUTTON_UP,
    BUTTON_DOWN
};

class Button {
    
private:
    
    int _x, _y;
    
    int _width, _height;
    
    bool _justPressed;
    
    Texture * _sprite;
    
    State _state;
    
    SDL_Rect _clipUp, _clipDown;

    std::function<void()> _pressed;
    
public:
    
    /**
     *  @param x The x location of the button.
     *  @param y The y location of the button.
     *  @param sprite The texture of the button.
     *  @param clipUp The clip of the sprite to render when the button is up.
     *  @param clipDown The clip of the sprite to render when the button is down.
     *  @param pressed A function to call when the button
     *                      has been pressed.
     */
    Button(int x, int y, Texture * sprite, SDL_Rect clipUp, SDL_Rect clipDown, std::function<void()> pressed);
    
    /**
     *  Renders the button.
     */
    void render();
    
    /**
     *  @param event An SDL_Event pointer that contains events to handle.
     */
    void handleEvent(SDL_Event * event);
};

#endif /* Button_hpp */
