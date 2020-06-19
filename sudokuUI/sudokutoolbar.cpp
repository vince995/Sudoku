#include "sudokutoolbar.h"

using namespace  std;

sudokuToolBar::sudokuToolBar(sf::Vector2f t_position, sf::Vector2f t_size) : m_generateGridButton(sf::Vector2f(0, 0), sf::Vector2f(100, 30), "generate"),
    m_solveGridButton(sf::Vector2f(0, 0), sf::Vector2f(100, 30), "solve grid"), m_solveCellButton(sf::Vector2f(0, 0), sf::Vector2f(100, 30), "solve cell"),
    isLoading(false)
{

    //init shape
    m_shape.setPosition(t_position);
    m_shape.setSize(t_size);
    m_shape.setFillColor(sf::Color::Cyan);


    //init buttons position in function of the toolbar position and size
    m_generateGridButton.setPosition(m_shape.getPosition().x, m_shape.getPosition().y + m_shape.getSize().y - m_generateGridButton.getSize().y);
    m_solveGridButton.setPosition(m_shape.getPosition().x+110, m_shape.getPosition().y + m_shape.getSize().y - m_solveGridButton.getSize().y);
    m_solveCellButton.setPosition(m_shape.getPosition().x+220, m_shape.getPosition().y + m_shape.getSize().y - m_solveCellButton.getSize().y);


    //set up the loading panda...
    if (!m_panda.loadFromFile("panda.png"))
    {
        cout << "fail \n";
    }

    m_pandaSprite.setTexture(m_panda);
    m_pandaSprite.setScale(sf::Vector2f(4, 4));
    m_pandaSprite.setPosition(sf::Vector2f(m_shape.getPosition().x + m_shape.getSize().x - m_pandaSprite.getLocalBounds().width*m_pandaSprite.getScale().x+4, m_shape.getPosition().y + m_shape.getSize().y - m_pandaSprite.getLocalBounds().height*m_pandaSprite.getScale().y+4));
    m_animationDir = make_tuple(1, 1, 1);


    //init progress text
    if (!m_progressFont.loadFromFile("Montserrat-Regular.ttf"))
    {
        cout << "fail \n";
    }

    m_progress.setFont(m_progressFont);
    m_progress.setString("0%");
    m_progress.setFillColor(sf::Color::Black);
    m_progress.setCharacterSize(35);
    m_progress.setPosition(sf::Vector2f(m_shape.getPosition().x + m_shape.getSize().x - m_pandaSprite.getLocalBounds().width*m_pandaSprite.getScale().x, m_shape.getPosition().y + m_shape.getSize().y - m_progress.getCharacterSize()));

}


void sudokuToolBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //draw stuff
    target.draw(m_generateGridButton, states);
    target.draw(m_solveGridButton, states);
    target.draw(m_solveCellButton, states);
    target.draw(m_pandaSprite, states);
    target.draw(m_progress, states);
    target.draw(m_pandaSprite, states);
}

void sudokuToolBar::update()
{


    //if the toolbar is in loading mode update the panda color else panda color is white
    if(isLoading){

        //animation speed management
        if(m_timer.getElapsedTime().asMicroseconds() > 100){

            m_timer.restart();
            int color = rand()%3;
            sf::Color currentColor = m_pandaSprite.getColor();




            //change of the direction of the animation when a component reach an extremum (255 or 0)
            if((currentColor.r+1 > 255 && get<0>(m_animationDir) == 1) or (currentColor.r-1 < 100 && get<0>(m_animationDir) == -1)){
                get<0>(m_animationDir) *= -1;
            }
            if((currentColor.g+2 > 255 && get<1>(m_animationDir) == 1) or (currentColor.g-2 < 100 && get<1>(m_animationDir) == -1)){
                get<1>(m_animationDir) *= -1;
            }
            if((currentColor.b+3 > 255 && get<2>(m_animationDir) == 1) or (currentColor.b-3 < 100 && get<2>(m_animationDir) == -1)){
                get<2>(m_animationDir) *= -1;
            }





            //update the colored panda in function of the animating components
            switch (color) {
                case 0:
                    m_pandaSprite.setColor(sf::Color((currentColor.r+get<0>(m_animationDir)), currentColor.g, currentColor.b));
                    break;
                case 1:
                    m_pandaSprite.setColor(sf::Color(currentColor.r, (currentColor.g+2*get<1>(m_animationDir)), currentColor.b));
                    break;
                case 2:
                    m_pandaSprite.setColor(sf::Color(currentColor.r, currentColor.g, (currentColor.b+3*get<2>(m_animationDir))));
                    break;

            }
        }


    }else{
        m_pandaSprite.setColor(sf::Color::White);
    }

}

void sudokuToolBar::setLoadingState(bool t_state)
{
    //update generate button in function of the loading state
    isLoading = t_state;
    if(isLoading){
        m_generateGridButton.setText("cancel");
    }else{
        m_generateGridButton.setText("generate");
    }
}

bool sudokuToolBar::getLoadingState()
{
    return isLoading;
}

void sudokuToolBar::setProgress(float t_progress)
{
    //update progress text to be right aligned
    m_progress.setString(to_string(static_cast<int>(floor(t_progress)))+"%");
    m_progress.setOrigin(m_progress.getLocalBounds().width, m_progress.getOrigin().y);
}

bool sudokuToolBar::getGenerateGridButtonState(int t_x, int t_y, mouseState t_mState)
{
    return m_generateGridButton.setMouseState(t_x, t_y, t_mState);
}

bool sudokuToolBar::getSolveGridButtonState(int t_x, int t_y, mouseState t_mState)
{
    return m_solveGridButton.setMouseState(t_x, t_y, t_mState);
}

bool sudokuToolBar::getSolveCellButtonState(int t_x, int t_y, mouseState t_mState)
{
    return m_solveCellButton.setMouseState(t_x, t_y, t_mState);
}


