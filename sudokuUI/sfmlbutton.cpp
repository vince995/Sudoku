#include "sfmlbutton.h"

using namespace std;

SFMLButton::SFMLButton(sf::Vector2f t_position, sf::Vector2f t_size, std::string t_text, bool t_toggleable, float t_ratio) : m_isPushed(false), m_isToggleable(t_toggleable), m_ratio(t_ratio)
{

    //init button style and shape
    m_style = SFMLButtonStyle();

    m_shape.setPosition(t_position);
    m_shape.setSize(t_size);
    m_shape.setFillColor(m_style.color);
    m_shape.setOutlineColor(m_style.outlineColor);
    m_shape.setOutlineThickness(m_style.outLineSize);


    //init text button
    if (!m_font.loadFromFile("OpenSans-Regular.ttf"))
    {
        cout << "fail \n";
    }

    m_txt.setFont(m_font);
    m_txt.setString("lp");
    m_txt.setFillColor(sf::Color::Black);
    m_txt.setCharacterSize(t_size.y*t_ratio);

    sf::FloatRect textRect = m_txt.getLocalBounds();
    m_txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

    m_txt.setPosition((int)(t_position.x + t_size.x/2.0f), (int)(t_position.y + t_size.y/2.0f));
    m_txt.setString(t_text);
    textRect = m_txt.getLocalBounds();
    m_txt.setOrigin(textRect.left + textRect.width/2.0f, m_txt.getOrigin().y);




}

SFMLButton::SFMLButton()
{

    //init button style and shape
    m_style = SFMLButtonStyle();

    m_shape.setPosition(sf::Vector2f(0, 0));
    m_shape.setSize(sf::Vector2f(50, 50));
    m_shape.setFillColor(m_style.color);
    m_shape.setOutlineColor(m_style.outlineColor);
    m_shape.setOutlineThickness(m_style.outLineSize);


    //init text button
    if (!m_font.loadFromFile("OpenSans-Regular.ttf"))
    {
        cout << "fail \n";
        // error...
    }

    m_txt.setFont(m_font);
    m_txt.setString(" ");
    m_txt.setFillColor(sf::Color::Black);
    m_txt.setCharacterSize(50/1.5f);

    sf::FloatRect textRect = m_txt.getLocalBounds();
    m_txt.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

    m_txt.setPosition(0 + 50/2.0f, 0 + 50/2.0f);

}

void SFMLButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    target.draw(m_shape, states);

    //if the button contains an image we just draw it, if not we draw the text
    if(m_containImage){
        target.draw(m_image_S, states);
    }else{
        target.draw(m_txt, states);
    }


}

void SFMLButton::setText(std::string t_text)
{
    m_txt.setString(t_text);

    //update x origine of the button to keep it centered
    sf::FloatRect textRect = m_txt.getLocalBounds();
    m_txt.setOrigin(textRect.left + textRect.width/2.0f, m_txt.getOrigin().y);

}

void SFMLButton::setImage(std::string t_path)
{

    //init image
    if (!m_image_I.loadFromFile(t_path))
    {
        cout << "fail \n";
    }

    m_containImage = true;
    m_image_T.loadFromImage(m_image_I);
    m_image_S.setTexture(m_image_T);


    //centered the image
    sf::FloatRect imgRect = m_image_S.getLocalBounds();
    m_image_S.setOrigin(imgRect.left + imgRect.width/2.0f, imgRect.top  + imgRect.height/2.0f);
    m_image_S.setPosition((int)(m_shape.getPosition().x + m_shape.getSize().x/2.0f), (int)(m_shape.getPosition().y + m_shape.getSize().y/2.0f));


}

void SFMLButton::setStyle(SFMLButtonStyle t_style)
{
    //apply style
    m_style = t_style;
    m_shape.setFillColor(m_style.color);
    m_shape.setOutlineColor(m_style.outlineColor);
    m_shape.setOutlineThickness(m_style.outLineSize);
}

void SFMLButton::setPosition(float t_x, float t_y)
{
    //set position and keep image and text centered on the button shape
    m_shape.setPosition(t_x, t_y);
    m_txt.setPosition(t_x + m_shape.getSize().x/2.0f, t_y + m_shape.getSize().y/2.0f);
    m_image_S.setPosition(t_x + m_shape.getSize().x/2.0f, t_y + m_shape.getSize().y/2.0f);
}

sf::Vector2f SFMLButton::getSize()
{
    return m_shape.getSize();
}

bool SFMLButton::contains(int t_x, int t_y)
{
    return m_shape.getGlobalBounds().contains(t_x, t_y);
}

buttonState SFMLButton::setMouseState(int t_x, int t_y, mouseState t_mState)
{



    //compute state for a normal button
    if(!m_isToggleable){

        m_longPushState = false;

        //if mouse is hovered the button...
        if(m_shape.getGlobalBounds().contains(t_x, t_y)){

            //if mouse state is clicked we set pushed state to true and start
            //the timer to detect a long push
            if(t_mState == CLICKED){

                m_isPushed = true;
                m_timer.restart();

            }
            //if the mouse state is none the button is just hovered or pushed
            else if(t_mState == NONE){

                //if the button state is pushed and the elapsed time since the click
                //exceeds one second we set long push state to true...
                if(m_timer.getElapsedTime().asSeconds() > 1 && m_isPushed){

                    cout << "long push detected \n";

                    m_longPushState = true;

                }

                //if the button is just hovered set the hovered style
                if(!m_isPushed){

                    m_shape.setFillColor(m_style.hoveredColor);
                    m_shape.setOutlineColor(m_style.outlineHoveredColor);

                }


            }
            //if the mouse is released over the button the button is no longer
            //pushed or longpushed so we set it to false and set default style
            else if (t_mState == RELEASED){

                m_isPushed = false;
                m_longPushState = false;
                m_shape.setFillColor(m_style.color);
                m_shape.setOutlineColor(m_style.outlineColor);

            }
        }
        //the button is not hovered, we set default style
        else{

            m_isPushed = false;
            m_shape.setFillColor(m_style.color);
            m_shape.setOutlineColor(m_style.outlineColor);

        }
    }
    //the button is toggleable...
    else{

        //if mouse is hovered the button...
        if(m_shape.getGlobalBounds().contains(t_x, t_y)){

            //if the mouse is clicked we start the timer to detect long push...
            if(t_mState == CLICKED){

                m_timer.restart();

            }
            //if the mouse state is none and the button is not pushed,
            //set the hovered style
            else if(t_mState == NONE){

                if(!m_isPushed){

                    m_shape.setFillColor(m_style.hoveredColor);
                    m_shape.setOutlineColor(m_style.outlineHoveredColor);

                }

            }


            else if (t_mState == RELEASED){

                //if the elapsed time since the click
                //exceeds 0.5 second we set long push state to true...
                if(m_timer.getElapsedTime().asSeconds() > 0.5 ){

                    cout << "long push detected \n";

                    m_longPushState = true;

                    //update over toggled state
                    if(m_isOverToggled){

                        //over toggle disable
                        m_isOverToggled = false;

                    }else{

                        //over toggle enable
                        m_isOverToggled = true;

                    }

                    t_mState = NONE;
                    m_isPushed = false;

                }
                //else we just update pushed state (in fact toggled or not)
                else {

                    if(m_isPushed){

                        m_isPushed = false;

                    }else{

                        m_isPushed = true;

                    }

                }

                //set default style if the button is not over toggled
                if(!m_isOverToggled){

                    m_shape.setFillColor(m_style.color);
                    m_shape.setOutlineColor(m_style.outlineColor);

                }
            }
        }
        //the button is not hovered, we set default style
        else{
            m_shape.setFillColor(m_style.color);
            m_shape.setOutlineColor(m_style.outlineColor);
        }
    }



    //set pushed style
    if(m_isPushed){

        m_shape.setFillColor(m_style.pushedColor);
        m_shape.setOutlineColor(m_style.outlinePushedColor);

    }


    //set over toggled style
    if(m_isOverToggled){
        m_shape.setFillColor(sf::Color(146, 219, 50));
        m_shape.setOutlineColor(sf::Color(146, 219, 50));
    }


    //if the mouse is within the button bounds,
    //mouse was released and the button is note over toggled we return a pushed state,
    //else if the button is in long push state we return it else we return default state.
    if(m_shape.getGlobalBounds().contains(t_x, t_y) && t_mState == RELEASED && !m_isOverToggled){

        cout << "button pushed \n";
        return PUSHED;

    }else if(m_longPushState){

        m_longPushState = false;
        return LONGPUSHED;

    }else{

        return DEFAULT;

    }

}

bool SFMLButton::getLongPushState()
{
    return m_longPushState;
}
