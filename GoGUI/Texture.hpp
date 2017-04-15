//
//  Texture.hpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

class Texture {
    
private:
    
    SDL_Texture * _texture;
    
    SDL_Renderer * _renderer;
        
    bool _isValid;
    
    /**
     *  @param path The path to the image for the texture.
     *  @return True if the image was loaded.
     */
    bool loadFrom(const char * path);

public:
    
    /**
     *  @param path The path to the image for the texture.
     *  @param renderer The renderer to render the texture to.
     */
    Texture(const char * path, SDL_Renderer * renderer);

    /**
     *  Destroy the texture.
     */
    ~Texture();
    
    /**
     *  @param x The x location to render the texture.
     *  @param y The y location to render the texture.
     *  @param textureClip The clip of the texture to render.
     */
    void render(int x, int y, SDL_Rect textureClip, SDL_Rect tgtClip);
    
    /**
     *  @return True if the texture is valid.
     */
    bool isValid() { return _isValid; }
};

#endif /* Texture_hpp */
