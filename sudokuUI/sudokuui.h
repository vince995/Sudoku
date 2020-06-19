#ifndef SUDOKUUI_H
#define SUDOKUUI_H


#include "sudokugrid.h"
#include "sudokutoolbar.h"
#include "padtoolbar.h"
#include "sfmlbutton.h"
#include "sfmlnumericpad.h"
#include "utilitary.h"

class sudokuUI : public sf::Drawable
{
public:
    sudokuUI();

    //update the ui in function of the mouse state and position
    void setMousePosition(float t_x, float t_y, mouseState t_clicked);

    //update some part of the ui (grid and main toolbar)
    void update(sf::Time t_elapsed);

    //transmits keyboard actions to the grid
    void keyPressed(int t_key);

    //draw function
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:

    //the "sudoku" title
    sf::Texture m_titleTexture;
    sf::RectangleShape m_title;

    //the different parts of the ui
    SFMLNumericPad m_numericPad;
    sudokuToolBar m_toolbar;
    padToolbar m_padToolbar;
    sudokuGrid m_sudokuGrid;


    //use to create the thread for the generation of a grid
    std::future<void> m_f;
};

#endif // SUDOKUUI_H
