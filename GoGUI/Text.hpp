//
//  Text.hpp
//  Go
//
//  Created by Ellis Hoag on 4/15/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Text_hpp
#define Text_hpp

#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"

class Text {
    
private:
    
    SDL_Renderer * _renderer;
    
    int _x, _y;
    
    TTF_Font * _font;
    
    SDL_Texture * _texture;
    
public:
    
    /**
     *  @param renderer A pointer to an SDL_Renderer
     *  @param x The x location of the text.
     *  @param y The y location of the text.
     *  @param font A pointer to the font.
     */
    Text(SDL_Renderer * renderer, int x, int y, TTF_Font * font);
    
    ~Text();
    
    /**
     *  @param text The string for the text.
     */
    void setText(const char * text);
    
    /**
     *  Renders the text.
     */
    void render();
};

#endif /* Text_hpp */
