//
//  Texture.cpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Texture.hpp"

using namespace std;

Texture::Texture(const char * path, SDL_Renderer * renderer) {
    
    _renderer = renderer;
    
    _isValid = loadFrom(path);
}

Texture::~Texture() {
    
    if (_texture) {
        SDL_DestroyTexture(_texture);
    }
}

bool Texture::loadFrom(const char * path) {
    
    SDL_Surface * surface = IMG_Load(path);
    if (!surface) {
        cout << "Error: " << IMG_GetError() << endl;
        return false;
    }
    
    _texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (!_texture) {
        cout << "Error: " << SDL_GetError() << endl;
        SDL_FreeSurface(surface);
        _texture = NULL;
        return false;
    }
    
    SDL_FreeSurface(surface);
    
    return true;
}

void Texture::render(int x, int y, SDL_Rect textureClip, SDL_Rect tgtClip) {
    
    SDL_RenderCopy(_renderer, _texture, &textureClip, &tgtClip);
}
