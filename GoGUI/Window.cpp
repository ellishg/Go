//
//  Window.cpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "Window.hpp"

using namespace std;

Window::Window(int width, int height) {
    
    _isValid = false;
    _isRunning = true;
    
    if(SDL_Init(SDL_INIT_VIDEO)) {
        
        cout << "Error initializing video.\nError: " << SDL_GetError() << endl;
        return;
    }
    
    _window = SDL_CreateWindow("Go", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    
    if(_window == NULL) {
        
        cout << "Error creating window.\nError: " << SDL_GetError() << endl;
        return;
    }
    
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        cout << "Error creating renderer.\nError: " << SDL_GetError() << endl;
        return;
    }
    
    _isValid = true;

}

Window::~Window() {
    
    if (_isValid) {
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }
}

bool Window::launchWindow() {
    
    while (_isRunning) {
        
        for (Texture * sprite : _textures) {
            sprite->render(<#int x#>, <#int y#>, <#SDL_Rect *textureClip#>)
        }
    }
}
