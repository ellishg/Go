//
//  main.cpp
//  GoGUI
//
//  Created by Ellis Hoag on 4/13/17.
//  Copyright © 2017 Ellis Hoag. All rights reserved.
//

#include "SDL2/SDL.h"

#include "Board.hpp"
#include "Window.hpp"

int main(int argc, const char * argv[]) {

    Window myWindow(300, 300);
    
    myWindow.launchWindow();
    
    return 0;
}
