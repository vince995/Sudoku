#include "padtoolbar.h"

padToolbar::padToolbar(sf::Vector2f t_position, sf::Vector2f t_size) : m_undoButton(sf::Vector2f(0, 0), sf::Vector2f(71, 45), "<-"),
m_eraseButton(sf::Vector2f(0, 0), sf::Vector2f(71, 45), "X"), m_noteButton(sf::Vector2f(0, 0), sf::Vector2f(71, 45), "/", true)
{

    //init shape
    m_shape.setPosition(t_position);
    m_shape.setSize(t_size);
    m_shape.setFillColor(sf::Color::Cyan);

    //init buttons position in function of the toolbar position and size
    m_undoButton.setPosition(m_shape.getPosition().x, m_shape.getPosition().y + m_shape.getSize().y - m_undoButton.getSize().y);
    m_eraseButton.setPosition(m_shape.getPosition().x+71, m_shape.getPosition().y + m_shape.getSize().y - m_eraseButton.getSize().y);
    m_noteButton.setPosition(m_shape.getPosition().x+142, m_shape.getPosition().y + m_shape.getSize().y - m_noteButton.getSize().y);


    //create button style
    SFMLButtonStyle style;
    style.color = sf::Color(220, 220, 220);
    style.pushedColor = sf::Color(200, 200, 200);
    style.hoveredColor = sf::Color(230, 230, 230);

    style.outlineColor = sf::Color(220, 220, 220);
    style.outlinePushedColor = sf::Color(200, 200, 200);
    style.outlineHoveredColor = sf::Color(220, 220, 220);

    style.outLineSize = -3;

    //set the style created to the buttons
    m_undoButton.setStyle(style);
    m_eraseButton.setStyle(style);
    m_noteButton.setStyle(style);


    //set the icons to the buttons
    m_undoButton.setImage("undo-button.png");
    m_eraseButton.setImage("erase_button.png");
    m_noteButton.setImage("note_button.png");

}

void padToolbar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    //draw the buttons
    target.draw(m_undoButton, states);
    target.draw(m_eraseButton, states);
    target.draw(m_noteButton, states);

}

/*void padToolbar::update()
{

}*/


buttonState padToolbar::getUndoButtonState(int t_x, int t_y, mouseState t_mState)
{
    return m_undoButton.setMouseState(t_x, t_y, t_mState);
}


buttonState padToolbar::getEraseButtonState(int t_x, int t_y, mouseState t_mState)
{
    return m_eraseButton.setMouseState(t_x, t_y, t_mState);
}

buttonState padToolbar::getNoteButtonState(int t_x, int t_y, mouseState t_mState)
{
    return m_noteButton.setMouseState(t_x, t_y, t_mState);
}

