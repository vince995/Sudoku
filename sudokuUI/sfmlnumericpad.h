#ifndef SFMLNUMERICPAD_H
#define SFMLNUMERICPAD_H


#include <SFML/Graphics.hpp>
#include "utilitary.h"
#include "sfmlbutton.h"

class SFMLNumericPad : public sf::Drawable
{
public:

    SFMLNumericPad(sf::Vector2f t_position, sf::Vector2f t_size);
    ~SFMLNumericPad();


    //if one of the numeric button is pushed return true and update
    //the state of the pad to the value of the button
    bool isClicked(int t_x, int t_y, mouseState t_mState);

    //return the state of the pad (the button pushed)
    unsigned int getState();

    //update mouse effect at the mouse position
    void updateMouseEffect(int t_x, int t_y);

    //draw function
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:

    //pad dimension
    sf::Vector2i m_size;
    sf::Vector2u m_position;
    sf::RectangleShape m_outline;


    //variable used for the mouse effect
    sf::Image m_mouseEffect_I;
    sf::Texture m_mouseEffect_T;
    sf::Sprite m_mouseEffect_S;
    int m_mouseEffectSize = 75;


    //buttons and style
    std::vector<SFMLButton*> m_numericButtons;
    SFMLButtonStyle m_buttonStyle;
    float m_buttonMargin = 12;


    //it's the number choose
    unsigned int m_state;




};

#endif // SFMLNUMERICPAD_H
