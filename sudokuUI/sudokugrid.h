#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include <thread>
#include <future>
#include <cmath>
#include <chrono>
#include <stack>
#include <fstream>
#include <experimental/filesystem>
#include "sudoku/sdkgenerator.h"


class sudokuGrid : public sf::Drawable
{
public:
    sudokuGrid();
    ~sudokuGrid();


    std::pair<sf::Vector2f, sf::Vector2f> getCoordinate();

    //recreate the game state with the previous grid played before game was closed
    bool loadFileGame(std::string t_path);


    //set the current hovered cell and focused cell
    void setFocusedCell(unsigned int t_x,unsigned int t_y, bool t_force = false);
    void setHoveredCell(unsigned int t_x,unsigned int t_y);
    void notHover();




    //update the different color for : focused, hovered, highlighted cell and errors higlighting
    //call also the functions to update the different effects
    void update();

    //check the validity of the notes with the current numbers entered on the grid
    void updateNotes(unsigned int t_value, unsigned int t_x,unsigned int t_y);
    //add and remove automatically the notes (using the updateNotes function)
    void updateAutoNotes();


    //function to handle the keyboard actions
    void keyboardAction(int t_key);


    //updates the state of the graphical grid with the SDKGrid passed in parameter
    void setSDKGrid(SDKGrid t_grid);

    //start a grid generation
    void generateRndSDKGrid();

    //send an abort message to the "thread"
    void abortGeneration();

    //return true if a grid is in generation
    bool isGeneratingGrid();

    //return the percentage of the generation grid progress
    float getGenerationProgress();



    //cheat functions
    void showSolvedGrid();
    void showSolvedCell();



    //setters and getters for manual and automatic note modes
    void setAutoNoteMode(bool t_autoNoteMode);
    bool getAutoNoteMode();

    void setManualNoteMode(bool t_noteMode);
    bool getManualNoteMode();

    

    //lock or unlock the grid
    void lockGrid(bool t_lock);



    //set or erase number
    void setNumber(unsigned int t_value, unsigned int t_x = 10, unsigned int t_y = 10);
    void eraseNumber();


    //check for invalid number entered and update the graphic grid in function of the result
    void showErrors(unsigned int t_value, unsigned int t_x, unsigned int t_y);
    bool rowError(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j);
    bool columnError(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j);
    bool blockError(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j);



    //return true if all the cells are filled in without checking for errors.
    bool isFullFilled();


    //clear functions...
    void clearColor();
    void clearGame();


    //update the mouse effect and the different animations
    void updateMouseEffect(int t_x, int t_y);
    void updateWinAnimation();
    void updateErrorAnimation();

    void undo();


    //draw function
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


private:


    //graphic grid position and dimension
    sf::Vector2u m_position;
    unsigned int m_cellSize;
    unsigned int m_gridSize;
    unsigned int m_outlineCellSize;


    //variables for the sprite that shows the different effects
    sf::Image m_effect_I;
    sf::Texture m_effect_T;
    sf::Sprite m_effect_S;

    int m_mouseEffectSize = 120;


    //different core grids used for generated grid and solution, and the current edited grid
    SDKGrid m_grid;
    SDKGrid m_solvedGrid;
    SDKGrid m_tmpGrid;

    //boolean vectors to handle notes
    std::vector<std::vector<std::vector<bool>>> m_notes;
    std::vector<std::vector<std::vector<bool>>> m_savedNotes;

    SDKGenerator m_generator;


    //table containing the grid cells state statut
    //first component is true if the cell is hovered
    //second component is true if the cell is focused
    //third component is true if the cell has error
    std::vector<std::vector<std::tuple<bool, bool, bool>>> m_ActionGrid;


    //position of different active cells state (see above)
    sf::Vector2u m_hoverPos;
    sf::Vector2u m_focusPos;
    sf::Vector2u m_errorPos;


    std::vector<std::vector<sf::RectangleShape>> m_gGrid;
    std::vector<std::vector<sf::RectangleShape>> m_blockGrid;
    sf::RectangleShape m_outline;


    //font used by the numbers
    sf::Font m_font;
    //table of numbers to display
    std::vector<std::vector<sf::Text>> m_gNumber;

    //font used by the notes
    sf::Font m_noteFont;
    //table of number note (81 cell of 9 number)
    std::vector<std::vector<std::vector<sf::Text>>> m_gNotes;


    //timer to set delay to keyboard actions
    sf::Clock m_keyDelay;

    bool m_isGeneratingGrid = false;
    bool m_isLocked = false;


    //the different error states
    bool m_errorFound = false;
    bool m_blockError = false;
    bool m_rowError = false;
    bool m_columnError = false;


    //note mode states
    bool m_manualNoteMode = false;
    bool m_autoNoteMode = false;

    //timer used to update animations
    sf::Clock m_timer;

    //stack of the different moves (number entered during the game), it's used by the undo function
    std::stack<std::pair<int, sf::Vector2u>> m_moveStack;

    //set to true if the grid is solved (manually or not)
    bool m_won = false;

};

#endif // SUDOKUGRID_H
