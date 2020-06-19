#include "sudokugrid.h"

using namespace std;


//resize the grid to the size given
template<typename T>
void initGrid(std::vector<std::vector<T>> &t_grid, unsigned int t_size){
    t_grid.resize(t_size);
    for(unsigned int i = 0; i < t_size; ++i){
        t_grid[i].resize(t_size);
    }
}

//fill a grid with the given value
template<typename T>
void fillGrid(std::vector<std::vector<T>> &t_grid, T t_value){
    for(unsigned int i = 0; i < t_grid.size(); ++i){
        for(unsigned int j = 0; j < t_grid[i].size(); ++j){
            t_grid[i][j] = t_value;
        }
    }
}

sudokuGrid::sudokuGrid() : m_tmpGrid(m_grid), m_hoverPos(0, 0), m_focusPos(0, 0)
{


    //init grid dimension
    m_cellSize = 60;
    m_gridSize = m_cellSize*9;
    m_outlineCellSize = 3;
    m_position.x = 175; m_position.y = 200;




    //loads the fonts used
    if (!m_font.loadFromFile("Montserrat-Regular.ttf"))
    {
        cout << "fail \n";
    }

    if (!m_noteFont.loadFromFile("BalooThambi2-Regular.ttf"))
    {
        cout << "fail \n";
    }


    //init the text for the numbers we have to draw :
    //initialization of the position according to the cell corresponding to it,
    //init also the origine (to be centered on the cell), the color etc...
    initGrid(m_gNumber, 9);

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            sf::Text number;
            number.setFont(m_font);
            number.setString("0");
            number.setCharacterSize(m_cellSize-10);
            number.setFillColor(sf::Color(120, 120, 120));

            //init position according to i and j value (in fact the current cell pos)
            number.setPosition(i*m_cellSize+m_position.x+m_cellSize/2, j*m_cellSize+m_position.y+m_cellSize/2);


            //center the text number
            sf::FloatRect textRect = number.getLocalBounds();
            number.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

            m_gNumber[i][j] = number;
        }
    }

    //just "erase the number for the moment
    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            m_gNumber[i][j].setString(" ");
        }
    }




    //init text for the notes (a little bit like above)
    initGrid(m_gNotes, 9);

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            m_gNotes[i][j].resize(9);
            int l = 0;
            for(unsigned int k = 0; k < 9; ++k){


                //we go to the line
                if(k%3 == 0){
                    l++;
                }

                sf::Text number;
                number.setFont(m_noteFont);
                number.setString(to_string(k+1));
                number.setCharacterSize(m_cellSize/3-2);
                number.setFillColor(sf::Color(100, 100, 100));

                //init position according to i/j for the current cell and k/l for the current number
                int x_ = i*m_cellSize+(k%3)*(m_cellSize/3-3)+m_cellSize/6+m_position.x+3;
                int y_ = j*m_cellSize+(l-1)*(m_cellSize/3-3)+m_cellSize/6+m_position.y+3;
                number.setPosition(x_, y_);

                //init origin to center the text note
                sf::FloatRect textRect = number.getLocalBounds();
                number.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

                m_gNotes[i][j][k] = number;






            }
        }
    }


    //init notes and saved notes grid both to false value
    initGrid(m_notes, 9);
    initGrid(m_savedNotes, 9);

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            m_notes[i][j].resize(9);
            m_savedNotes[i][j].resize(9);
            for(unsigned int k = 0; k < 9; ++k){
                m_notes[i][j][k] = false;
                m_savedNotes[i][j][k] = false;
            }
        }
    }




    //init grid action to false
    initGrid(m_ActionGrid, 9);
    fillGrid(m_ActionGrid, make_tuple(false, false, false));



    //init graphic cells
    initGrid(m_gGrid, 9);

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            sf::RectangleShape cell;
            cell.setOutlineThickness(m_outlineCellSize/2);
            cell.setOutlineColor(sf::Color(0, 0, 0, 60));
            cell.setSize(sf::Vector2f(m_cellSize-m_outlineCellSize, m_cellSize-m_outlineCellSize));
            cell.setPosition(i*m_cellSize+m_position.x+m_outlineCellSize/2, j*m_cellSize+m_position.y+m_outlineCellSize/2);
            m_gGrid[i][j] = cell;
        }
    }



    //init graphic blocks (to represent 3*3 cells)
    initGrid(m_blockGrid, 3);

    for(unsigned int i = 0; i < 3; ++i){
        for(unsigned int j = 0; j < 3; ++j){
            sf::RectangleShape block;
            block.setOutlineThickness(2);
            block.setOutlineColor(sf::Color(50, 50, 50, 200));
            block.setFillColor(sf::Color::Transparent);
            block.setSize(sf::Vector2f(m_cellSize*3, m_cellSize*3));
            block.setPosition(i*m_cellSize*3+m_position.x, j*m_cellSize*3+m_position.y);
            m_blockGrid[i][j] = block;
        }
    }


    //the outline of the graphic grid
    m_outline.setOutlineThickness(-2);
    m_outline.setOutlineColor(sf::Color::Black);
    m_outline.setSize(sf::Vector2f(m_gridSize, m_gridSize));
    m_outline.setPosition(m_position.x, m_position.y);
    m_outline.setFillColor(sf::Color::Transparent);





    //focus the center
    setFocusedCell(4*m_cellSize+m_position.x, 4*m_cellSize+m_position.y);


    //init effect sprite
    m_effect_I.create(m_gridSize, m_gridSize, sf::Color::Red);
    m_effect_T.loadFromImage(m_effect_I);
    m_effect_S.setTexture(m_effect_T);
    m_effect_S.setPosition(m_position.x, m_position.y);


    //load last game played
    loadFileGame("sudoku_saved.sdk");
}

sudokuGrid::~sudokuGrid()
{

    //save the current grid in the save file
   ofstream file("sudoku_saved.sdk");
   for(unsigned int i = 0; i < 9; ++i){

       for(unsigned int j = 0; j < 9; ++j){

           //if the cell was edited we put P (for player) after its value else we put G (for game)
           if(m_grid(i, j) != 0 or m_tmpGrid(i, j) == 0){

               file << m_tmpGrid(i, j) << "G";

           }else{

               file << m_tmpGrid(i, j) << "P";

           }

           //save the solved grid
           file << m_solvedGrid(i, j) << " ";
       }

       file << "\n";

   }

   file << "\n";

   //save the current notes...
   for(unsigned int i = 0; i < 9; ++i){

       for(unsigned int j = 0; j < 9; ++j){

            for(unsigned int k = 0; k < 9; ++k){

                file << m_notes[i][j][k];

            }

            file << " ";

       }

       file << "\n";

   }

}

void sudokuGrid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    //draw effect sprite
    target.draw(m_effect_S, states);

    //draw graphic grid : cells, blocks and outline
    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            target.draw(m_gGrid[i][j], states);
        }
    }

    for(unsigned int i = 0; i < 3; ++i){
        for(unsigned int j = 0; j < 3; ++j){
            target.draw(m_blockGrid[i][j], states);
        }
    }   

    target.draw(m_outline);


    //draw numbers
    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            target.draw(m_gNumber[i][j], states);
        }
    }

    //draw notes
    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            for(unsigned int k = 0; k < 9; ++k){
                if(m_notes[i][j][k]){
                    target.draw(m_gNotes[i][j][k], states);
                }
            }
        }
    }
}


bool sudokuGrid::loadFileGame(std::string t_path)
{
    //creates local grids
    SDKGrid grid;
    SDKGrid tmpGrid;

    std::vector<std::vector<std::vector<bool>>> notes;
    initGrid(notes, 9);

    //open save files
    ifstream file(t_path);
    if(!file.is_open()){
        return false;
    }


    //variable used for iteration through the save file
    string line;
    unsigned int i = 0;
    unsigned int j = 0;


    bool noteStep = false; //if true start to load number notes
    int pas = 4; //number of character discribing a cell state plus one


    //iteration line by line
    while (getline(file, line)) {


        if(!noteStep){
            //iterate on all characters of the line
            while (j < line.size()/pas) {

                //put value on grid or tmp grid according to G or P state of the cell
                if(line[j*pas+1] == 'G'){

                    grid(i, j) = line[j*pas]-48;

                }else{

                    tmpGrid(i, j) = line[j*pas]-48;

                }

                m_solvedGrid(i, j) = line[j*pas+2]-48;

                ++j;

            }

            j = 0;

        }else{

            //iterate on all characters of the line and fill notes grid
            while (j < line.size()/pas) {

                notes[i-1][j].resize(9);
                for(unsigned int k = 0; k < 9; ++k){

                    notes[i-1][j][k] = line[j*pas+k]-48;

                }

                ++j;

            }

            j = 0;

        }

        //if line is empty start to fill note grid
        if(line.size() == 0){
            noteStep = true;
            pas = 10;
            i = 0;
        }


        ++i;
    }

    i = 0;

    //apply the grid loaded from save file
    setSDKGrid(grid);


    //check for errors
    bool error = false;
    int x_ = 0;
    int y_ = 0;

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(tmpGrid(i, j) != 0){
                setNumber(tmpGrid(i, j), i, j);
                if(m_errorFound){
                    error = true;
                    x_ = i;
                    y_ = j;
                }
            }
        }
    }

    if(error){
        setFocusedCell(x_*m_cellSize+m_position.x, y_*m_cellSize+m_position.y, true);
        showErrors(tmpGrid(x_, y_), x_, y_);
    }
    cout << "error found : " << m_errorFound << endl;


    //clear history
    while (!m_moveStack.empty()) {
        m_moveStack.pop();
    }

    //apply notes loaded
    m_notes = notes;

    return true;

}


std::pair<sf::Vector2f, sf::Vector2f> sudokuGrid::getCoordinate()
{
    return std::make_pair(m_outline.getPosition(), m_outline.getSize());
}

void sudokuGrid::setHoveredCell(unsigned int t_x,unsigned int t_y)
{
    //if the cell is not already hovered
    if((t_x-m_position.x)/m_cellSize != m_hoverPos.x or (t_y-m_position.y)/m_cellSize != m_hoverPos.y){

        //update hover cell state
        get<0>(m_ActionGrid[(t_x-m_position.x)/m_cellSize][(t_y-m_position.y)/m_cellSize]) = true;
        get<0>(m_ActionGrid[m_hoverPos.x][m_hoverPos.y]) = false;

        //update hover position
        m_hoverPos.x = (t_x-m_position.x)/m_cellSize;
        m_hoverPos.y = (t_y-m_position.y)/m_cellSize;
    }
}

void sudokuGrid::notHover()
{
    get<0>(m_ActionGrid[m_hoverPos.x][m_hoverPos.y]) = false;
}


void sudokuGrid::setFocusedCell(unsigned int t_x, unsigned int t_y, bool t_force)
{
    //focus the cell if the game is note won and there is no error, but if t_force is set to true force the focus
    if((!m_errorFound && !m_won) or t_force){
        if((t_x-m_position.x)/m_cellSize != m_focusPos.x or (t_y-m_position.y)/m_cellSize != m_focusPos.y){

            //update focus cell state
            get<1>(m_ActionGrid[floor(t_x-m_position.x)/m_cellSize][floor(t_y-m_position.y)/m_cellSize]) = true;
            get<1>(m_ActionGrid[m_focusPos.x][m_focusPos.y]) = false;

            //update focus position
            m_focusPos.x = (t_x-m_position.x)/m_cellSize;
            m_focusPos.y = (t_y-m_position.y)/m_cellSize;

        }
    }
}

void sudokuGrid::updateNotes(unsigned int t_value, unsigned int t_x, unsigned int t_y)
{

    //if the number is in the column, delete the corresponding note
    for(unsigned int t_column = 0; t_column < 9; ++t_column){
        m_notes[t_x][t_column][t_value-1] = false;
    }

    //if the number is in the row, delete the corresponding note
    for(unsigned int t_row = 0; t_row < 9; ++t_row){
        m_notes[t_row][t_y][t_value-1] = false;
    }


    //if the number is in the block, delete the corresponding note
    unsigned int i_ = t_x-(t_x%3);
    unsigned int j_ = t_y-(t_y%3);

    for(unsigned int i = i_ ; i < i_+3; ++i){
        for(unsigned int j = j_ ; j < j_+3; ++j){
            m_notes[i][j][t_value-1] = false;
        }
    }

    //delete notes in the current pos (because there is a number)
    for(unsigned int i = 0; i < 9; ++i){
        m_notes[t_x][t_y][i] = false;
    }

}


void sudokuGrid::updateAutoNotes()
{
    //update all note to true
    for(unsigned int i = 0 ; i < 9; ++i){
        for(unsigned int j = 0 ; j < 9; ++j){
            if(m_tmpGrid(i, j) == 0){
                for(unsigned k = 0; k < 9; ++k){
                    m_notes[i][j][k] = true;
                }
            }
        }
    }

    //use "updateNotes" to delete invalid notes
    for(unsigned int i = 0 ; i < 9; ++i){
        for(unsigned int j = 0 ; j < 9; ++j){

            if(m_tmpGrid(i, j) != 0){
                updateNotes(m_tmpGrid(i, j), i, j);
            }

        }
    }




}

void sudokuGrid::update()
{
    if(!m_isLocked){
        //clear color of cells
        for(unsigned int i = 0; i < 9; ++i){
            for(unsigned int j = 0; j < 9; ++j){
                if(!m_won){
                    m_gGrid[i][j].setFillColor(sf::Color::White);
                }else{
                    m_gGrid[i][j].setFillColor(sf::Color(255, 255, 255, 230));
                }



            }
        }



        //update color of cell focused and highlighted
        for(unsigned int i = 0; i < 9; ++i){
            for(unsigned int j = 0; j < 9; ++j){


                if(get<1>(m_ActionGrid[i][j]) == true){


                    //highlight the 3*3 block
                    unsigned int i_ = i-(i%3);
                    unsigned int j_ = j-(j%3);

                    for(unsigned int k = i_ ; k < i_+3; ++k){
                        for(unsigned int l = j_ ; l < j_+3; ++l){

                            if(m_blockError){
                                get<2>(m_ActionGrid[k][l]) = true;
                            }else{
                                m_gGrid[k][l].setFillColor(sf::Color(225, 225, 255));
                            }

                        }
                    }




                    //highlight the vertical and horizontal lines
                    for(unsigned int k = 0; k < 9; ++k){

                        if(m_columnError){
                            get<2>(m_ActionGrid[i][k]) = true;
                        }else{
                            m_gGrid[i][k].setFillColor(sf::Color(225, 225, 255));
                        }

                    }
                    for(unsigned int k = 0; k < 9; ++k){
                        if(m_rowError){
                            get<2>(m_ActionGrid[k][j]) = true;
                        }else{
                            m_gGrid[k][j].setFillColor(sf::Color(225, 225, 255));
                        }

                    }

                    //color the focused cell
                    m_gGrid[i][j].setFillColor(sf::Color(200, 200, 255));
                }
            }
        }


        //highlight error cell
        if(m_errorFound){
            for(unsigned int k = 0 ; k < 9; ++k){
                for(unsigned int l = 0 ; l < 9; ++l){
                    if(get<2>(m_ActionGrid[k][l]) == true){
                        m_gGrid[k][l].setFillColor(sf::Color(255, 225, 225));
                    }
                }
            }

            m_gGrid[m_focusPos.x][m_focusPos.y].setFillColor(sf::Color(255, 200, 200));
        }



        //update color of cell hovered, highlighted or not
        for(unsigned int i = 0; i < 9; ++i){
            for(unsigned int j = 0; j < 9; ++j){

                if(get<0>(m_ActionGrid[i][j])== true){

                    if(m_focusPos.x == i || m_focusPos.y == j || (m_focusPos.x-(m_focusPos.x%3) <= i && i < m_focusPos.x-(m_focusPos.x%3)+3 && m_focusPos.y-(m_focusPos.y%3) <= j && j < m_focusPos.y-(m_focusPos.y%3)+3)){
                        m_gGrid[i][j].setFillColor(sf::Color(210, 210, 255));
                    }else{
                        m_gGrid[i][j].setFillColor(sf::Color(225, 225, 225));
                    }


                }

            }
        }
    }


    //if the grid is full filled and there is no error the game is won
    if(isFullFilled() && !m_errorFound){

        updateWinAnimation();
        m_won = true;

    }else{

        m_won = false;

    }

    //if an error was found update error animation
    if(m_errorFound){
        updateErrorAnimation();
    }




}

void sudokuGrid::keyboardAction(int t_key)
{


    //We check the conditions to write the entered number :

    if(!m_isLocked){

        if(m_keyDelay.getElapsedTime().asMicroseconds() > 20000){

            //tab key pressed, focus the next cell
            if(t_key == 60){

                if(m_focusPos.x > 7 && m_focusPos.y > 7){
                    setFocusedCell(m_position.x, m_position.y);
                }
                else if(m_focusPos.x > 7){
                    setFocusedCell(m_position.x, (m_focusPos.y+1)*m_cellSize+m_position.y);
                }else if(m_focusPos.y > 8){
                    setFocusedCell((m_focusPos.x+1)*m_cellSize+m_position.x, m_position.y);
                }else{
                    setFocusedCell((m_focusPos.x+1)*m_cellSize+m_position.x, m_focusPos.y*m_cellSize+m_position.y);
                }


            }
            //left key pressed focus the next cell on the left
            else if(t_key == 71){
                if(m_focusPos.x < 1){
                    setFocusedCell(8*m_cellSize+m_position.x, (m_focusPos.y)*m_cellSize+m_position.y);
                }else{
                    setFocusedCell((m_focusPos.x-1)*m_cellSize+m_position.x, m_focusPos.y*m_cellSize+m_position.y);
                }

                cout << "left \n";
            }
            //right key pressed focus the next cell on the right
            else if(t_key == 72){
                if(m_focusPos.x > 7){
                    setFocusedCell(m_position.x, (m_focusPos.y)*m_cellSize+m_position.y);
                }else{
                    setFocusedCell((m_focusPos.x+1)*m_cellSize+m_position.x, m_focusPos.y*m_cellSize+m_position.y);
                }
                cout << "right \n";
            }
            //down key pressed focus the cell below
            else if(t_key == 73){

                if(m_focusPos.y < 1){
                    setFocusedCell(m_focusPos.x*m_cellSize+m_position.x, 8*m_cellSize+m_position.y);
                }else{
                    setFocusedCell(m_focusPos.x*m_cellSize+m_position.x, (m_focusPos.y-1)*m_cellSize+m_position.y);
                }
                cout << "down \n";
            }
            //up key pressed focus the cell above
            else if(t_key == 74){
                if(m_focusPos.y > 7){
                    setFocusedCell(m_focusPos.x*m_cellSize+m_position.x, m_position.y);
                }else{
                    setFocusedCell(m_focusPos.x*m_cellSize+m_position.x, (m_focusPos.y+1)*m_cellSize+m_position.y);
                }
                cout << "up \n";
            }
            //another key is pressed check if it's a number or not
            else if(!m_isLocked and m_grid(m_focusPos.x, m_focusPos.y) == 0){
                if(t_key-75 > 0 && t_key-75 < 10){
                    setNumber(t_key-75, m_focusPos.x, m_focusPos.y);
                }else{
                    setNumber(0, m_focusPos.x, m_focusPos.y);
                }

            }

            m_keyDelay.restart();
        }
    }





}

void sudokuGrid::setSDKGrid(SDKGrid t_grid)
{


    m_errorFound = false;

    m_grid = t_grid;
    m_tmpGrid = t_grid;

    clearColor();

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(m_grid(i, j)){
                m_gNumber[i][j].setString(to_string(m_grid(i, j)));
                m_gNumber[i][j].setFillColor(sf::Color(70, 70, 70));
            }else{
                m_gNumber[i][j].setString(" ");
                m_gNumber[i][j].setFillColor(sf::Color(120, 120, 255));
            }

        }
    }



}


void sudokuGrid::generateRndSDKGrid()
{


    //lock the grid and update generating statut
    m_isGeneratingGrid = true;
    lockGrid(true);

    cout << "started" << endl;

    //use the SDKGenerator
    pair<SDKGrid, SDKGrid> result = m_generator.create();

    //check if the generation hasn't been aborted
    //if not reset and update game with the new grid
    if(result.first(0, 0) != 10){

        setSDKGrid(result.first);
        m_solvedGrid = result.second;

        for(unsigned int i = 0; i < 9; ++i){
            for(unsigned int j = 0; j < 9; ++j){
                for(unsigned int k = 0; k < 9; ++k){
                    m_savedNotes[i][j][k] = m_notes[i][j][k] = false;
                }
            }
        }

        showErrors(0, m_focusPos.x, m_focusPos.y);

        while (!m_moveStack.empty()) {
            m_moveStack.pop();
        }

        if(m_autoNoteMode){
            updateAutoNotes();
        }

        m_won = false;
    }


    cout << "finished" << endl;

    //unlock grid and update generating statut
    m_isGeneratingGrid = false;
    lockGrid(false);


}

void sudokuGrid::abortGeneration()
{
    //send abort signal to the grid generator
    m_generator.abort();
}

void sudokuGrid::showSolvedGrid()
{
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(m_tmpGrid(i, j) == 0){
                setNumber(m_solvedGrid(i, j), i, j);
            }
        }
    }

}

void sudokuGrid::showSolvedCell()
{
    setNumber(m_solvedGrid(m_focusPos.x, m_focusPos.y), m_focusPos.x, m_focusPos.y);
}

void sudokuGrid::setAutoNoteMode(bool t_autoNoteMode)
{

    //update note mode
    m_autoNoteMode = t_autoNoteMode;
    m_manualNoteMode = false;

    if(m_autoNoteMode){

        //save user notes and first call to "updateAutoNotes"
        m_savedNotes = m_notes;
        updateAutoNotes();

    }else{
        //set notes saved when auto note mode was actived
        for(unsigned int i = 0 ; i < 9; ++i){
            for(unsigned int j = 0 ; j < 9; ++j){

                if(m_tmpGrid(i, j) == 0){
                    for(unsigned k = 0; k < 9; ++k){
                        m_notes[i][j][k] = m_savedNotes[i][j][k];
                    }
                }
            }
        }

    }
}

bool sudokuGrid::getAutoNoteMode()
{
    return m_autoNoteMode;
}

float sudokuGrid::getGenerationProgress()
{
    return m_generator.getProgress();
}

bool sudokuGrid::isGeneratingGrid()
{
    return m_isGeneratingGrid;
}

void sudokuGrid::lockGrid(bool t_lock)
{
    m_isLocked = t_lock;

    if(t_lock){
        m_outline.setFillColor(sf::Color(0, 0, 0, 60));
    }else{
        m_outline.setFillColor(sf::Color(0, 0, 0, 0));
    }
}

void sudokuGrid::setManualNoteMode(bool t_noteMode)
{
    m_manualNoteMode = t_noteMode;
}

bool sudokuGrid::getManualNoteMode()
{
    return m_manualNoteMode;
}

void sudokuGrid::setNumber(unsigned int t_value, unsigned int t_x, unsigned int t_y)
{

    //not valid pos was given so we use focused cell pos
    if(t_x == 10 or t_y == 10){
        t_x = m_focusPos.x;
        t_y = m_focusPos.y;
    }


    //if manual note mode isn't active set number update graphic grid and notes etc...
    if(!m_manualNoteMode){
        if(t_value != 0){

            m_moveStack.push(make_pair(t_value, sf::Vector2u(t_x, t_y)));

            m_tmpGrid(t_x, t_y) = t_value;
            m_gNumber[t_x][t_y].setString(to_string(t_value));


            updateNotes(t_value, t_x, t_y);
        }else{
            m_gNumber[t_x][t_y].setString("");
            m_tmpGrid(t_x, t_y) = 0;
        }

        showErrors(t_value, t_x, t_y);
    }
    //else if note mode is active set note if it's valid
    else {
        if(t_value != 0){
            if(m_notes[t_x][t_y][t_value-1]){
                m_notes[t_x][t_y][t_value-1] = false;
            }else{
                if(SDKSolver::isValid(t_value, m_tmpGrid, t_x, t_y)){
                    m_notes[t_x][t_y][t_value-1] = true;
                }
            }
        }
    }

    //if auto note mode is active update whole notes
    if(m_autoNoteMode){
        updateAutoNotes();
        cout << "update auto note \n";
    }

}

void sudokuGrid::eraseNumber()
{
    //if it's not a "game" number delete it
    if(m_grid(m_focusPos.x, m_focusPos.y) == 0){

        setNumber(0, m_focusPos.x, m_focusPos.y);

        //if auto note mode is active update whole notes
        if(m_autoNoteMode){
            updateAutoNotes();
        }
    }
}

void sudokuGrid::showErrors(unsigned int t_value, unsigned int t_x, unsigned int t_y)
{

    clearColor();

    //check for row and column error
    m_rowError = rowError(t_value, m_tmpGrid, t_x, t_y);
    m_columnError = columnError(t_value, m_tmpGrid, t_x, t_y);


    //if there is row or column errors we don't account for block errors
    if(m_rowError or m_columnError){
        m_blockError = false;
    }else{
        m_blockError = blockError(t_value, m_tmpGrid, t_x, t_y);
    }


    //update errorFound if there is no column, row or block error
    if(!(m_rowError or m_columnError or m_blockError)){
        m_errorFound = false;
        clearColor();
    }





}


bool sudokuGrid::rowError(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j)
{
    bool rowError = false;


    if(t_value != 0){

        //iterate through column
        for(unsigned int t_column = 0; t_column < 9; ++t_column){

            //if the value is found on the column set column error to true
            if(t_grid(t_column, t_j) == t_value && (t_column != t_i) && t_value != 0){
                m_gNumber[t_column][t_j].setFillColor(sf::Color::Red);
                rowError = true;
                m_errorFound = true;
            }
        }

        if(rowError){
            m_gNumber[t_i][t_j].setFillColor(sf::Color::Red);
            cout << "row error \n";
            return true;
        }
    }


    return false;

}

bool sudokuGrid::columnError(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j)
{

    bool columnError = false;

    if(t_value != 0){

        //iterate through row
        for(unsigned int t_row = 0; t_row < 9; ++t_row){

            //if the value is found on the row set row error to true
            if(t_grid(t_i, t_row) == t_value && (t_row != t_j) && t_value != 0){
                m_gNumber[t_i][t_row].setFillColor(sf::Color::Red);
                columnError = true;
                m_errorFound = true;
            }
        }

        if(columnError){
            m_gNumber[t_i][t_j].setFillColor(sf::Color::Red);
            cout << "column error \n";
            return true;
        }
    }


    return false;

}

bool sudokuGrid::blockError(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j)
{
    unsigned int i_ = t_i-(t_i%3);
    unsigned int j_ = t_j-(t_j%3);

    bool blockError = false;

    if(t_value != 0){

        //iterate through block
        for(unsigned int i = i_ ; i < i_+3; ++i){
            for(unsigned int j = j_ ; j < j_+3; ++j){

                //if the value is found on the block set block error to true
                if((t_grid(i, j) == t_value) && (t_i != i or t_j != j) && t_value != 0){
                    m_gNumber[i][j].setFillColor(sf::Color::Red);
                    blockError = true;
                    m_errorFound = true;
                }
            }
        }

        if(blockError){
            m_gNumber[t_i][t_j].setFillColor(sf::Color::Red);
            cout << "block error \n";
            return true;
        }
    }


    return false;


}


void sudokuGrid::clearColor()
{
    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(m_grid(i, j) != 0){
                m_gNumber[i][j].setFillColor(sf::Color(70, 70, 70));
            }else{
                m_gNumber[i][j].setFillColor(sf::Color(120, 120, 255));
            }

            get<2>(m_ActionGrid[i][j]) = false;
        }
    }
}

void sudokuGrid::clearGame()
{
    cout << "clear game ! \n";
    setSDKGrid(m_grid);
}


void sudokuGrid::updateMouseEffect( int t_x,  int t_y)
{

    //translate the position to start at (0, 0)
    t_x -= m_position.x;
    t_y -= m_position.y;

    //clear the image
    m_effect_I.create(m_gridSize, m_gridSize, sf::Color::Transparent);


    int lowerBoundary = ceil(static_cast<float>(m_outlineCellSize)/2);

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
            float colorCpt = dist/m_mouseEffectSize*155+100;


            //check if the pixel is in the circle of "m_mouseEffectSize" radius
            if (dist < m_mouseEffectSize){

                //check if it's on the sprite dimensions
                if(i >= 0 && i < m_gridSize && j >= 0 && j < m_gridSize){


                    //draw the effect just on the visible part of the sprite
                    if(!(i%m_cellSize < m_cellSize-lowerBoundary && i%m_cellSize >= m_outlineCellSize/2)
                            or !(j%m_cellSize < m_cellSize-lowerBoundary && j%m_cellSize >= m_outlineCellSize/2)){

                        m_effect_I.setPixel(i, j, sf::Color(colorCpt, colorCpt, colorCpt));


                    }

                }

            }
        }
    }

    m_effect_T.update(m_effect_I);
}


void sudokuGrid::updateWinAnimation()
{

    //use timer
    float pas = m_timer.getElapsedTime().asSeconds()*300;


    //loop over a square of grid dimension and centered
    for(int i = 0; i < m_gridSize; ++i){

        //horizontal distance between the current pixel and the center position
        int x_ = i-m_focusPos.x*m_cellSize-m_cellSize/2;

        for(int j = 0; j < m_gridSize; ++j){

             //vertical distance between the current pixel and the center position
            int y_ = j-m_focusPos.y*m_cellSize-m_cellSize/2;

            //compute distance between the current pixel and the center position
            float dist = sqrt(x_*x_+y_*y_);

            //compute color in function of the distance (to make the gradient)
            float colorCpt = (sin((dist-pas)/50)+1)/2*200+20;

            //check if it's on the sprite dimensions
            if(i >= 0 && i < m_gridSize && j >= 0 && j < m_gridSize){
                m_effect_I.setPixel(i, j, sf::Color(colorCpt, 255, colorCpt));
            }
        }
    }

    m_effect_T.update(m_effect_I);

}

void sudokuGrid::updateErrorAnimation()
{

    //use timer
    float pas = m_timer.getElapsedTime().asSeconds()*300;

    int lowerBoundary = ceil(static_cast<float>(m_outlineCellSize)/2);

    //loop over a square of grid dimension and centered
    for(int i = 0; i < m_gridSize; ++i){

        //horizontal distance between the current pixel and the center position
        int x_ = i-m_focusPos.x*m_cellSize-m_cellSize/2;

        for(int j = 0; j < m_gridSize; ++j){

            //vertical distance between the current pixel and the center position
            int y_ = j-m_focusPos.y*m_cellSize-m_cellSize/2;

            //compute distance between the current pixel and the center position
            float dist = sqrt(x_*x_+y_*y_);

            //compute color in function of the distance (to make the gradient)
            float colorCpt = (sin((dist-pas)/50)+1)/2*50+205;

            //check if it's on the sprite dimensions
            if(i >= 0 && i < m_gridSize && j >= 0 && j < m_gridSize){

                //draw the effect just on the visible part of the sprite
                if(!(i%m_cellSize < m_cellSize-lowerBoundary && i%m_cellSize >= m_outlineCellSize/2)
                        or !(j%m_cellSize < m_cellSize-lowerBoundary && j%m_cellSize >= m_outlineCellSize/2)){

                    m_effect_I.setPixel(i, j, sf::Color(255, colorCpt, colorCpt));

                }


            }
        }
    }

    m_effect_T.update(m_effect_I);

}


bool sudokuGrid::isFullFilled()
{
    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(m_tmpGrid(i, j) == 0){
                return false;
            }
        }
    }

    return true;
}

void sudokuGrid::undo()
{
    cout << m_moveStack.size() << endl;

    //check if history is empty
    if(!m_moveStack.empty()){

        //erase the last number entered
        setNumber(0, m_moveStack.top().second.x, m_moveStack.top().second.y);
        m_moveStack.pop();

        if(!m_moveStack.empty()){

            //put the penultimate back on the grid
            setNumber(m_moveStack.top().first, m_moveStack.top().second.x, m_moveStack.top().second.y);
            showErrors(m_moveStack.top().first, m_moveStack.top().second.x, m_moveStack.top().second.y);

            //check for error after undo action
            if(m_errorFound){
                cout << "error after undo" << endl;
                setFocusedCell((m_moveStack.top().second.x*m_cellSize+m_position.x), (m_moveStack.top().second.y*m_cellSize+m_position.y), true);
            }
            m_moveStack.pop();
        }

    }

}
