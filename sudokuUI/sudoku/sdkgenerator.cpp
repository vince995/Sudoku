#include <fstream>
#include "sdkgenerator.h"

using namespace std;

SDKGenerator::SDKGenerator()
{
    srand(time(nullptr));
}

void SDKGenerator::abort()
{
    m_aborted = true;
}

bool SDKGenerator::setRemainingNb(unsigned int t_value)
{
    if(t_value >= 0 && t_value < 55){
        m_remainingNb = t_value;
        return true;
    }
    return false;
}

float SDKGenerator::getProgress()
{
    return m_progress;
}

void SDKGenerator::fillBox(unsigned int t_i, unsigned int t_j)
{
    for(unsigned int k = 1; k <=9; ++k){
        int x = rand()%3 + t_i*3;
        int y = rand()%3 + t_j*3;
        while (m_grid(x, y) != 0) {
            x = rand()%3 + t_i*3;
            y = rand()%3 + t_j*3;
        }
        m_grid(x, y) = k;
    }
}

void SDKGenerator::fillDiagonal()
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        fillBox(i, i);
    }
}

std::pair<SDKGrid, SDKGrid> SDKGenerator::create()
{
    //we do this to avoid problem on windows
    srand(time(nullptr));

    pair<SDKGrid, SDKGrid> result;
    m_grid.reset(0);

    fillDiagonal();

    //solve the created grid
    SDKSolver solver;
    solver.solve(m_grid, false);

    //to return the solved grid...
    result.second = m_grid;

    //put cells in a vector
    vector<std::pair<int, int>> cell;

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            cell.push_back(pair<int, int>(i, j));
        }
    }


    //
    for(unsigned int i = 0; i < 81-m_remainingNb && !m_aborted; ++i){
        int pos = 0;
        int value = 0;
        bool found = false;


        //try to delete a value at a random position and check if the grid
        //has still a unique solution
        do{

            //if all cell was check we stop the loop
            //never mind if m_remainingNb was not reached
            if(cell.size() == 0){
                break;
            }


            pos = rand()%cell.size();
            value = m_grid(cell[pos].first, cell[pos].second);
            m_grid(cell[pos].first, cell[pos].second) = 0;


            if(!solver.multipleSolution(m_grid)){

                cell.erase(cell.begin()+pos);
                found = true;

            }else{

                m_grid(cell[pos].first, cell[pos].second) = value;
                cell.erase(cell.begin()+pos);
                found = false;

            }

        }
        //we loop until no value is found or generation is aborted
        while(!found && !m_aborted);

        //a value to delete was found so we update the progress
        m_progress = i/static_cast<float>(80-m_remainingNb)*100;

        cout << i/static_cast<float>(80-m_remainingNb)*100 << endl;

    }

    cout << "final grid : \n";


    //write the grid created in a file
    ofstream file("sudoku2.txt");

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            file << m_grid(j, i);
        }
        file << "\n";
    }

    if(m_aborted){
        m_grid.reset(10);
    }

    m_aborted = false;

    result.first = m_grid;

    result.first.print();
    result.second.print();

    return result;

}
