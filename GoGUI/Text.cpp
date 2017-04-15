//
//  Text.cpp
//  Go
//
//  Created by Ellis Hoag on 4/15/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Text.hpp"

Text::Text(SDL_Renderer * renderer, int x, int y, TTF_Font * font) {
    
    _renderer = renderer;
    _x = x;
    _y = y;
    _font = font;
    _texture = NULL;
    
    setText("NULL");
}

Text::~Text() {
    if (!_texture) {
        SDL_DestroyTexture(_texture);
    }
}

void Text::setText(const char * text) {
    
    SDL_Surface * surface = TTF_RenderText_Solid(_font, text, {0, 0, 0, 255});
    
    if (!_texture) {
        SDL_DestroyTexture(_texture);
    }
    _texture = SDL_CreateTextureFromSurface(_renderer, surface);
    
    SDL_FreeSurface(surface);
}

void Text::render() {
    
    int width;
    int height;
    SDL_QueryTexture(_texture, NULL, NULL, &width, &height);
    
    SDL_Rect location = {_x, _y, width, height};
    
    SDL_RenderCopy(_renderer, _texture, nullptr, &location);
}
