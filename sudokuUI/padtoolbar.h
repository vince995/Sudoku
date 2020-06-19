#ifndef PADTOOLBAR_H
#define PADTOOLBAR_H

#include <utilitary.h>
#include <tuple>
#include "sfmlbutton.h"

class padToolbar : public sf::Drawable
{
public:
    padToolbar(sf::Vector2f t_position, sf::Vector2f t_size);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    //nothing to update right now
    //void update();


    //get the buttons state
    buttonState getUndoButtonState(int t_x, int t_y, mouseState t_mState);
    buttonState getEraseButtonState(int t_x, int t_y, mouseState t_mState);
    buttonState getNoteButtonState(int t_x, int t_y, mouseState t_mState);

private:
    //shape of the toolbar
    sf::RectangleShape m_shape;


    //buttons
    SFMLButton m_undoButton;
    SFMLButton m_eraseButton;
    SFMLButton m_noteButton;

};

#endif // PADTOOLBAR_H
