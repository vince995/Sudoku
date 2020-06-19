#include <iostream>
#include <SFML/Graphics.hpp>

#include <chrono>

#include "sudokuui.h"


using namespace std;

int main()
{

    srand(time(nullptr));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SUDOKU", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sudokuUI app;

    sf::View view(sf::Vector2f(500, 400), sf::Vector2f(1000, 800));
    window.setView(view);



    mouseState mouseClickState = NONE;

    sf::Clock clock;

    sf::Vector2i oldSize = sf::Vector2i(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {


        mouseClickState = NONE;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();


            if (event.type == sf::Event::KeyPressed)
            {
                cout << event.key.code << endl;
                app.keyPressed(event.key.code);
            }


            if (event.type == sf::Event::Resized)
            {

                sf::Vector2f ratioChange((float)event.size.width/(float)oldSize.x, (float)event.size.height/(float)oldSize.y);
                cout << ratioChange.x << " " << ratioChange.y << endl;
                cout << oldSize.x << " " << oldSize.y << endl;
                oldSize = sf::Vector2i(event.size.width, event.size.height);
                view.setSize(view.getSize().x*ratioChange.x, view.getSize().y*ratioChange.y);
                window.setView(view);

            }

            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    mouseClickState = CLICKED;
                }else if(event.mouseButton.button == sf::Mouse::Right){
                    mouseClickState = RIGHTCLIKED;
                }
            }else if(event.type == sf::Event::MouseButtonReleased){

                if(event.mouseButton.button == sf::Mouse::Left){
                    mouseClickState = RELEASED;
                }
            }


        }



        // get the current mouse position in the window
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

        // convert it to game coordinates
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        app.setMousePosition(worldPos.x, worldPos.y, mouseClickState);


        app.update(clock.restart());

        window.clear(sf::Color::White);
        window.draw(app);
        window.display();


    }

    return 0;
}
