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
#include "SDL2_ttf/SDL_ttf.h"
#include <vector>

#include "Button.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

class Window {
    
private:
    
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    
    std::vector<Button> _buttons;
    
    std::vector<Sprite *> _sprites;
    
    std::vector<Text *> _texts;
    
    std::function<void(int, int)> _onMouseClick;
    
    std::function<void()> _onLoop;
    
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
    void addButton(Button button) { _buttons.push_back(button); }
    
    /**
     *  Adds sprite to the window.
     *  @param sprite The sprite to add to the window.
     */
    void addSprite(Sprite * sprite) { _sprites.push_back(sprite); }
    
    /**
     *  @param text The text to add to the window.
     */
    void addText(Text * text) { _texts.push_back(text); }
    
    /**
     * @param onMouseClick The Function to run when the mouse has clicked.
     */
    void setMouseClick(std::function<void(int, int)> onMouseClick) {
        _onMouseClick = onMouseClick;
    }
    
    /**
     *  Launch the window with the current settings.
     *  This will not return until the window has closed.
     */
    bool launchWindow();
    
    /**
     *  @return True if the window was sucessfully constructed.
     */
    bool isValid() { return _isValid; }
    
    /**
     *  @return A pointer to the renderer.
     */
    SDL_Renderer * getRenderer() { return _renderer; }
    
    /**
     *  @param onLoop The function to call before every frame.
     */
    void setOnLoop(std::function<void()> onLoop) { _onLoop = onLoop; }
    
    /**
     *  Closes the window.
     */
    void quit() {
        _isRunning = false;
    }
};

#endif /* Window_hpp */
