//
//  Sprite.cpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Sprite.hpp"

using namespace std;

Sprite::Sprite(Texture * texture, vector<SDL_Rect> rects) {
    
    _texture = texture;
    _rects = rects;
    state = 0;
}

void Sprite::render() {
    
    _texture->render(x, y, _rects[state], {x, y, width, height});
}
