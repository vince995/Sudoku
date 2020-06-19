#ifndef SUDOKUTOOLBAR_H
#define SUDOKUTOOLBAR_H

#include <SFML/Graphics.hpp>
#include <tuple>

#include "sfmlbutton.h"

class sudokuToolBar : public sf::Drawable
{
public:
    sudokuToolBar(sf::Vector2f t_position, sf::Vector2f t_size);



    //just update the "panda" animation if a grid is in generation
    void update();

    //function in relation to the generation state of the grid
    void setLoadingState(bool t_state);
    void setProgress(float t_progress);
    bool getLoadingState();

    //get the buttons state
    bool getGenerateGridButtonState(int t_x, int t_y, mouseState t_mState);
    bool getSolveGridButtonState(int t_x, int t_y, mouseState t_mState);
    bool getSolveCellButtonState(int t_x, int t_y, mouseState t_mState);

    //draw function
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:

    //shape of the toolbar
    sf::RectangleShape m_shape;

    //panda loading
    sf::Texture m_panda;
    std::tuple<int, int, int> m_animationDir;
    sf::Sprite m_pandaSprite;


    //buttons
    SFMLButton m_generateGridButton;
    SFMLButton m_solveGridButton;
    SFMLButton m_solveCellButton;

    //progress displaying
    sf::Font m_progressFont;
    sf::Text m_progress;


    //timer for animation
    sf::Clock m_timer;

    //animation state
    bool isLoading = false;
};

#endif // SUDOKUTOOLBAR_H
