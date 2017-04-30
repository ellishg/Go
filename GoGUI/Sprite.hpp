//
//  Sprite.hpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Sprite_hpp
#define Sprite_hpp

#include <vector>

#include "Texture.hpp"

class Sprite {
    
private:
    
    std::vector<SDL_Rect> _rects;
    
public:

    Texture * _texture;

    int x, y;
    
    int width, height;
    
    int state;
    
    /**
     *  @param texture A pointer to a texture for the sprite.
     *  @param rects A vector of SDL_Rect such that rects[i] 
     *                  is the SDL_Rect of the texture to render
     *                  for state i.
     */
    Sprite(Texture * texture, std::vector<SDL_Rect> rects);
    
    /**
     *  Renders the sprite.
     */
    void render();

};

#endif /* Sprite_hpp */
