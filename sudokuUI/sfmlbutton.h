#ifndef SFMLBUTTON_H
#define SFMLBUTTON_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utilitary.h"

//make an enum for the different return state of a button
enum buttonState {
    PUSHED,
    LONGPUSHED,
    DEFAULT
};

//a structure to encapsulate the appearance of the button
struct SFMLButtonStyle {
    int outLineSize = -2;

    sf::Color outlineColor = sf::Color(175, 175, 250);
    sf::Color outlineHoveredColor = sf::Color(175, 175, 250);
    sf::Color outlinePushedColor = sf::Color(150, 150, 225);

    sf::Color color = sf::Color(200, 200, 250);
    sf::Color hoveredColor = sf::Color(175, 175, 250);
    sf::Color pushedColor = sf::Color(150, 150, 225);
};



class SFMLButton : public sf::Drawable
{
public:
    SFMLButton();
    SFMLButton(sf::Vector2f t_position, sf::Vector2f t_size, std::string t_text, bool t_toggleable = false, float t_ratio = 0.6);



    //the different setters
    void setText(std::string t_text);
    void setImage(std::string t_path);
    void setStyle(SFMLButtonStyle t_style);
    void setPosition(float t_x, float t_y);


    //return true if the coordinates are in the buttons bounds
    bool contains(int t_x, int t_y);

    //return true if the button is "long pushed"
    bool getLongPushState();

    //return size of the button
    sf::Vector2f getSize();

    //apply hover, pushed, longpuhed or default style to the button and return its state
    buttonState setMouseState(int t_x, int t_y, mouseState t_mState);


    //draw function
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::RectangleShape m_shape;

    //text of the button
    sf::Font m_font;
    sf::Text m_txt;

    //icon of the button
    sf::Image m_image_I;
    sf::Texture m_image_T;
    sf::Sprite m_image_S;

    sf::Clock m_timer;

    SFMLButtonStyle m_style;


    //state of the button
    bool m_isPushed = false;
    bool m_longPushState = false;

    bool m_isToggleable = false;
    bool m_isOverToggled = false;

    bool m_containImage = false;


    //ration between the text and button size
    float m_ratio;


};

#endif // SFMLBUTTON_H
