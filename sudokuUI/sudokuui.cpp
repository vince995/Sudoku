#include <iostream>
#include "sudokuui.h"

using namespace std;

sudokuUI::sudokuUI() : m_numericPad(sf::Vector2f(750, 200), sf::Vector2f(201, 201)), m_toolbar(sf::Vector2f(175, 110), sf::Vector2f(540, 70)),
    m_padToolbar(sf::Vector2f(750-6, 410), sf::Vector2f(201, 45))
{

    //init of m_title
    if (!m_titleTexture.loadFromFile("sudoku_title.png"))
    {
        cout << "fail to load title image \n";

    }
    m_title.setSize(sf::Vector2f(400, 100));
    m_title.setPosition(sf::Vector2f(250, 0));
    m_title.setFillColor(sf::Color::White);
    m_title.setTexture(&m_titleTexture);

}

void sudokuUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //we draw the different parts of the ui
    target.draw(m_title, states);
    target.draw(m_toolbar, states);
    target.draw(m_padToolbar, states);
    target.draw(m_sudokuGrid, states);
    target.draw(m_numericPad, states);
}

void sudokuUI::setMousePosition(float t_x, float t_y, mouseState t_clicked)
{

    //check if the mouse is within the grid dimentions
    sf::Vector2f topLeft = m_sudokuGrid.getCoordinate().first;
    sf::Vector2f bottomRight = sf::Vector2f(m_sudokuGrid.getCoordinate().second.x + topLeft.x, m_sudokuGrid.getCoordinate().second.y + topLeft.y);
    bool isInFrame = t_x > topLeft.x && t_x < bottomRight.x && t_y > topLeft.y && t_y < bottomRight.y;

    //here we check the status of the erase and note buttons because we need to see if they're just "pushed" or "long pushed".
    buttonState eraseButtonState = m_padToolbar.getEraseButtonState(t_x, t_y, t_clicked);
    buttonState noteButtonState = m_padToolbar.getNoteButtonState(t_x, t_y, t_clicked);


    //all the actions that can be trigged by the mouse state and position
    if(isInFrame && t_clicked == CLICKED){

        m_sudokuGrid.setFocusedCell(t_x, t_y);

    }else if(isInFrame && t_clicked == NONE){

        m_sudokuGrid.setHoveredCell(t_x, t_y);

    }else if(m_toolbar.getGenerateGridButtonState(t_x, t_y, t_clicked) == PUSHED){

        //if there is no grid in generation we generate one in a thread otherwise we send him an abandon signal to the thread
        if(!m_sudokuGrid.isGeneratingGrid()){

            m_f = std::async(std::launch::async, &sudokuGrid::generateRndSDKGrid, &m_sudokuGrid);

        }else {

            m_sudokuGrid.abortGeneration();

        }

    }else if(m_toolbar.getSolveGridButtonState(t_x, t_y, t_clicked) == PUSHED){

        m_sudokuGrid.showSolvedGrid();

    }else if(m_toolbar.getSolveCellButtonState(t_x, t_y, t_clicked) == PUSHED){

        m_sudokuGrid.showSolvedCell();

    }else if(eraseButtonState == PUSHED){

        m_sudokuGrid.eraseNumber();

    }else if(eraseButtonState == LONGPUSHED){

        m_sudokuGrid.clearGame();

    }else if(m_padToolbar.getUndoButtonState(t_x, t_y, t_clicked) == PUSHED){

        m_sudokuGrid.undo();

    }else if(t_clicked == RIGHTCLIKED && isInFrame){

        //right click erase the number in the cell hovered
        m_sudokuGrid.setFocusedCell(t_x, t_y);
        m_sudokuGrid.eraseNumber();

    }else if(noteButtonState == PUSHED){

        //enable or disable the manual note mode
        if(m_sudokuGrid.getManualNoteMode()){

            cout << "manual note mode disabled! \n";
            m_sudokuGrid.setManualNoteMode(false);
        }else{

            cout << "manual note mode enabled! \n";
            m_sudokuGrid.setManualNoteMode(true);

        }

    }else if(noteButtonState == LONGPUSHED){

        //enable or disable the automatic note mode
        if(m_sudokuGrid.getAutoNoteMode()){
            cout << "auto note disabled! \n";
            m_sudokuGrid.setAutoNoteMode(false);
        }else{
            cout << "auto note enabled! \n";
            m_sudokuGrid.setAutoNoteMode(true);
        }

    }else if(m_numericPad.isClicked(t_x, t_y, t_clicked) && !m_sudokuGrid.isGeneratingGrid()){

        //enter the number clicked on the pad on the grid
        m_sudokuGrid.setNumber(m_numericPad.getState());

    }else if(!isInFrame){

        m_sudokuGrid.notHover();

    }


    //updates the aesthetic effects
    m_numericPad.updateMouseEffect(t_x, t_y);
    m_sudokuGrid.updateMouseEffect(t_x, t_y);





}

void sudokuUI::update(sf::Time t_elapsed)
{
    m_toolbar.update();
    m_sudokuGrid.update();


    m_toolbar.setLoadingState(m_sudokuGrid.isGeneratingGrid());
    m_toolbar.setProgress(m_sudokuGrid.getGenerationProgress());
}

void sudokuUI::keyPressed(int t_key){
    m_sudokuGrid.keyboardAction(t_key);
}
