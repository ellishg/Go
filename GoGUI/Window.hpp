//
//  Window.hpp
//  Go
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <iostream>
#include "SDL2/SDL.h"
#include <vector>

#include "Button.hpp"
#include "Texture.hpp"

class Window {
    
private:
    
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    
    std::vector<Button *> _buttons;
    
    std::vector<Texture *> _textures;
    
    bool _isValid;
    bool _isRunning;
    
    /**
     *  Renderes all buttons and textures.
     */
    void _render();
    
    /**
     *  Handles all events of all buttons and
     *      all user defined events.
     */
    void _onEvent();
    
public:
    
    /**
     *  Creates a window of size width by height.
     *  @param width The width of the window.
     *  @param height The height of the window.
     */
    Window(int width, int height);
    
    /**
     *  Destroys the window.
     */
    ~Window();
    
    /**
     *  Adds button to the window.
     *  @param button The button to add to the window.
     */
    void addButton(Button * button) { _buttons.push_back(button); }
    
    /**
     *  Adds sprite to the window.
     *  @param sprite The sprite to add to the window.
     */
    void addTexture(Texture * sprite) { _textures.push_back(sprite); }
    
    /**
     *
     */
    void addEvent();
    
    /**
     *  Launch the window with the current settings.
     *  This will not return until the window has closed.
     */
    bool launchWindow();
    
    /**
     *  @return True if the window was sucessfully constructed.
     */
    bool isValid() { return _isValid; }
    
    SDL_Renderer * getRenderer() { return _renderer; }
};

#endif /* Window_hpp */
