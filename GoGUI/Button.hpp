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
#include "Sprite.hpp"

enum State {
    BUTTON_UP = 0,
    BUTTON_DOWN = 1
};

class Button {
    
private:
                
    Sprite _sprite;
        
    std::function<void()> _pressed;
    
public:
    
    /**
     *  @param tgt An SDL_Rect representing the location of the button.
     *  @param texture The texture of the button.
     *  @param clipUp The clip of the sprite to render when the button is up.
     *  @param clipDown The clip of the sprite to render when the button is down.
     *  @param pressed A function to call when the button
     *                      has been pressed.
     */
    Button(SDL_Rect tgt, Texture * texture, SDL_Rect clipUp, SDL_Rect clipDown, std::function<void()> pressed);
    
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
