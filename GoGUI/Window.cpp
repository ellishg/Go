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
    
    if (SDL_Init(SDL_INIT_VIDEO)) {
        
        cout << "Error: " << SDL_GetError() << endl;
        return;
    }
    
    if (TTF_Init() == -1) {
        cout << "Error: " << SDL_GetError() << endl;
        return;
    }
    
    _window = SDL_CreateWindow("Go", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    
    if (_window == NULL) {
        
        cout << "Error: " << SDL_GetError() << endl;
        return;
    }
    
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);
    if (!_renderer) {
        cout << "Error: " << SDL_GetError() << endl;
        return;
    }
    
    // Select the color for drawing. It is set to red here.
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    
    // Clear the entire screen to our selected color.
    SDL_RenderClear(_renderer);
    
    _isValid = true;

}

Window::~Window() {
    
    if (_isValid) {
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }
}

void Window::_render() {
    
    SDL_RenderClear(_renderer);
    
    for (Sprite * sprite : _sprites) {
        sprite->render();
    }
    
    for (Text * text : _texts) {
        text->render();
    }
    
    for (Button button : _buttons) {
        button.render();
    }
    
    SDL_RenderPresent(_renderer);

}

void Window::_onEvent() {
    
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        
        switch (event.type) {
            case SDL_QUIT:
                _isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        _isRunning = false;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                _onMouseClick(mouseX, mouseY);
                break;
            default:
                break;
        }
    
        for (Button button : _buttons) {
            button.handleEvent(&event);
        }
    }
}

bool Window::launchWindow() {
    
    while (_isValid && _isRunning) {
        
        _onEvent();
        
        _onLoop();
        
        _render();
    }
    
    return true;
}
