#include <any>
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"
#include "title_screen.hpp"
#include "gameplay.hpp"
#include "gamescreen.hpp"

// initialize globals
int FRAMERATE = 60;
double DELTATIME = 1.0 / (double) FRAMERATE; // tie physics to framerate

/*
Create the main gamescreen and run the game loop.
*/
int main()
{

    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project"};
    window.setFramerateLimit(FRAMERATE);
    window.setVerticalSyncEnabled(true); // call it once, after creating the gamescreen

    // track current active gamescreen, starting with title screen
    auto windowState = WindowState::TITLE_SCREEN;

    // make gamescreen classes using pointers
    TitleScreen* titleScreen = new TitleScreen();
    GamePlay* gamePlay = new GamePlay();

    // put gamescreen classes into map
    std::map<WindowState, GameScreen*> windowMap;
    windowMap[WindowState::TITLE_SCREEN] = (GameScreen*)titleScreen;
    windowMap[WindowState::GAMEPLAY] = (GameScreen*)gamePlay;
    
    // set the active gamescreen
    auto activeWindow = windowMap[windowState];

    while (window.isOpen()){

        // loop through events
        for (auto event = sf::Event{}; window.pollEvent(event);){

            // main-level events
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    // handle input in active gamescreen
                    auto handleInputFlags = activeWindow->handleInput(event);

                    // check bitwise handle input flags
                    if (handleInputFlags.HasFlag((EFlagValue)HandleInputActions::QUIT)){
                        window.close();
                    }
                    if (handleInputFlags.HasFlag((EFlagValue)HandleInputActions::CHANGEACTIVEGAMESCREENTITLESCREEN)){
                        windowState = WindowState::TITLE_SCREEN;
                        activeWindow = windowMap[windowState];
                    }
                    if (handleInputFlags.HasFlag((EFlagValue)HandleInputActions::CHANGEACTIVEGAMESCREENGAMEPLAY)){
                        windowState = WindowState::GAMEPLAY;
                        activeWindow = windowMap[windowState];
                    }
                    break;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw
        activeWindow->draw(window);

        // display
        window.display();
    }
}