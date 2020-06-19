#include "sfmlnumericpad.h"

using namespace std;

SFMLNumericPad::SFMLNumericPad(sf::Vector2f t_position, sf::Vector2f t_size) : m_size(t_size), m_position(t_position)
{

    //init mouse effect
    m_mouseEffect_I.create(t_size.x, t_size.y, sf::Color(0, 0, 0, 100));
    m_mouseEffect_T.loadFromImage(m_mouseEffect_I);
    m_mouseEffect_S.setTexture(m_mouseEffect_T);
    m_mouseEffect_S.setPosition(m_position.x, m_position.y);




    //init buttons style
    m_buttonStyle.color = sf::Color::White;
    m_buttonStyle.hoveredColor = sf::Color(215, 215, 215, 128);
    m_buttonStyle.pushedColor = sf::Color(80, 80, 80, 50);

    m_buttonStyle.outlineColor = sf::Color::Transparent;
    m_buttonStyle.outlineHoveredColor = sf::Color(100, 100, 100, 50);
    m_buttonStyle.outlinePushedColor = sf::Color(100, 100, 100, 50);
    m_buttonStyle.outLineSize = 4;




    //init outline
    m_outline.setSize(sf::Vector2f(t_size.x+m_buttonMargin, t_size.y+m_buttonMargin));
    m_outline.setPosition(sf::Vector2f(t_position.x-m_buttonMargin/2, t_position.y-m_buttonMargin/2));
    m_outline.setOutlineThickness(-2);
    m_outline.setOutlineColor(sf::Color(200, 200, 200));



    //init button (set style and position)
    int line = 0;
    for(unsigned int i = 0; i < 9; ++i){

        if(i%3 == 0){
            line++;
        }


        SFMLButton* b = new SFMLButton(sf::Vector2f(t_position.x+(i%3)*t_size.x/3+m_buttonMargin/2, t_position.y+(line-1)*(t_size.y/3)+m_buttonMargin/2), sf::Vector2f(t_size.x/3-m_buttonMargin, t_size.y/3-m_buttonMargin), to_string(i+1), false, 0.5);
        b->setStyle(m_buttonStyle);
        m_numericButtons.push_back(b);
    }

}

SFMLNumericPad::~SFMLNumericPad(){
    for(unsigned int i = 0; i < 9; ++i){
        delete m_numericButtons[i];
    }
}

void SFMLNumericPad::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    target.draw(m_outline, states);

    for(unsigned int i = 0; i < 9; ++i){
        target.draw(*m_numericButtons[i], states);
    }

    target.draw(m_mouseEffect_S, states);
}

bool SFMLNumericPad::isClicked(int t_x, int t_y, mouseState t_mState)
{

    //we loop through the 9 buttons and if one is pushed we return true and set the state to its value
    bool clicked = false;

    for(unsigned int i = 0; i < 9; ++i){

        if(!clicked && m_numericButtons[i]->setMouseState(t_x, t_y, t_mState) == PUSHED){

            clicked = true;
            m_state = i+1;

            cout << "button " << m_state << " used \n";

            break;
        }

    }

    return  clicked;

}

unsigned int SFMLNumericPad::getState()
{
    return m_state;
}


void SFMLNumericPad::updateMouseEffect( int t_x,  int t_y)
{

    //translate the position to start at (0, 0)
    t_x -= m_position.x;
    t_y -= m_position.y;

    //clear the image
    m_mouseEffect_I.create(m_size.x, m_size.y, sf::Color::Transparent);

    //loop over a square of "m_mouseEffectSize" dimension and centered on mouse position
    for( int i = t_x-m_mouseEffectSize; i < t_x+m_mouseEffectSize; ++i){

        //horizontal distance between the current pixel and the mouse position
        int x_ = i-t_x;

        for( int j = t_y-m_mouseEffectSize; j < t_y+m_mouseEffectSize; ++j){

            //vertical distance between the current pixel and the mouse position
            int y_ = j-t_y;

            //compute distance between the current pixel and the mouse position
            float dist = sqrt(x_*x_+y_*y_);

            //compute color in function of the distance (to make the gradient)
            float colorCpt = dist/m_mouseEffectSize*100+155;


            //check if the pixel is in the circle of "m_mouseEffectSize" radius
            if (dist < m_mouseEffectSize){

                //check if it's on the sprite dimensions
                if(i >= 0 && i < m_size.x && j >= 0 && j < m_size.y){


                    //draw the effect just on the visible part of the sprite
                    if(!(i%(m_size.x/3) < (m_size.x/3)-m_buttonMargin/2 && i%(m_size.x/3) >= m_buttonMargin/2)
                            or !(j%(m_size.x/3) < (m_size.x/3)-m_buttonMargin/2 && j%(m_size.x/3) >= m_buttonMargin/2)){

                        int outline = m_buttonStyle.outLineSize;

                        if((i%(m_size.x/3) < (m_size.x/3)-m_buttonMargin/2+outline && i%(m_size.x/3) >= m_buttonMargin/2-outline)
                                && (j%(m_size.x/3) < (m_size.x/3)-m_buttonMargin/2+outline && j%(m_size.x/3) >= m_buttonMargin/2-outline)){

                                m_mouseEffect_I.setPixel(i, j, sf::Color(0, 0, 0, 255-colorCpt));

                        }
                    }
                }
            }
        }
    }

    m_mouseEffect_T.update(m_mouseEffect_I);

}

