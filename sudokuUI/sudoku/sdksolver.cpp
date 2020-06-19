#include "sdksolver.h"

using namespace std;

SDKSolver::SDKSolver()
{

}


unsigned int SDKSolver::solve(SDKGrid &t_grid, bool t_countSolution)
{
    m_orderedCell = orderCell(t_grid);

    if(t_countSolution){

        unsigned int solution = 0;

        return solution;

    }else{

        fill(t_grid, m_orderedCell.begin());
        return 1;

    }

}

bool SDKSolver::multipleSolution(SDKGrid t_grid)
{
    m_orderedCell = orderCell(t_grid);
    unsigned int solution = 0;

    countSolution(t_grid, m_orderedCell.begin(), solution, 2);

    if(solution < 2){
        return false;
    }

    return true;
}




bool SDKSolver::isOnTheRow(unsigned int t_value, SDKGrid t_grid, unsigned int t_row)
{
    for(unsigned int t_column = 0; t_column < 9; ++t_column){
        if(t_grid(t_column, t_row) == t_value){
            return true;
        }
    }
    return false;
}

bool SDKSolver::isOnTheColumn(unsigned int t_value, SDKGrid t_grid, unsigned int t_column)
{
    for(unsigned int t_row = 0; t_row < 9; ++t_row){
        if(t_grid(t_column, t_row) == t_value){
            return true;
        }
    }
    return false;
}

bool SDKSolver::isOnTheBlock(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j)
{
    unsigned int i_ = t_i-(t_i%3);
    unsigned int j_ = t_j-(t_j%3);

    for(unsigned int i = i_ ; i < i_+3; ++i){
        for(unsigned int j = j_ ; j < j_+3; ++j){
            if(t_grid(i, j) == t_value){
                return true;
            }
        }
    }

    return false;

}


bool SDKSolver::isValid(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j)
{
    return !isOnTheRow(t_value, t_grid, t_j) && !isOnTheColumn(t_value, t_grid, t_i) && !isOnTheBlock(t_value, t_grid, t_i, t_j);
}


bool SDKSolver::fill(SDKGrid &t_grid, std::multimap<int, std::pair<int, int>>::iterator t_pos)
{

    int i = t_pos->second.first;
    int j = t_pos->second.second;

    for(int k = 1; k <= 9; ++k){

        if(distance(m_orderedCell.begin(), t_pos) == m_orderedCell.size()){

            return true;
        }

        if(isValid(k, t_grid, i, j)){

            t_grid(i, j) = k;

            if(fill(t_grid, ++t_pos)){

                return true;

            }

            t_grid(i, j) = 0;
            t_pos--;
        }

    }

    return false;
}

bool SDKSolver::countSolution(SDKGrid t_grid, std::multimap<int, std::pair<int, int> >::iterator t_pos, unsigned int &t_count, unsigned int t_limite)
{
    int i = t_pos->second.first;
    int j = t_pos->second.second;

    for(int k = 1; k <= 9; ++k){

        if(distance(m_orderedCell.begin(), t_pos) == m_orderedCell.size()-1){

            if(isValid(k, t_grid, i, j)){

                 if(t_count >= t_limite-1){

                     t_count++;

                     return true;

                 }

                 t_count++;
                 t_grid(i, j) = k;

            }

        }else{

            if(isValid(k, t_grid, i, j)){

                t_grid(i, j) = k;

                if(countSolution(t_grid, ++t_pos, t_count, t_limite)){

                    return true;

                }

                t_grid(i, j) = 0;
                t_pos--;

            }
        }
    }

    return false;
}

std::multimap<int, std::pair<int, int>> SDKSolver::orderCell(SDKGrid t_grid)
{
    SDKGrid possibilityGrid;
    possibilityGrid.reset(9);

    /*for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(t_grid(i, j) != 0){

                for(unsigned int c = 0; c < 9; ++c){
                    if(possibilityGrid(i, c) > 0){
                        --possibilityGrid(i, c);
                    }
                }
                for(unsigned int r = 0; r < 9; ++r){
                    if(possibilityGrid(r, j) > 0){
                        --possibilityGrid(r, j);
                    }
                }

                unsigned int r_ = i-(i%3);
                unsigned int c_ = j-(j%3);

                for(unsigned int r = r_ ; r < r_+3; ++r){
                    for(unsigned int c = c_ ; c < c_+3; ++c){
                        if(possibilityGrid(r, c) > 0){
                            --possibilityGrid(r, c);
                        }
                    }
                }
            }
        }
    }*/


    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(t_grid(i, j) != 0){
                possibilityGrid(i, j) = 0;
            }
        }
    }

    std::multimap<int, std::pair<int, int>> orderedCell;

    for(unsigned int i = 0; i < 9; ++i){
        for(unsigned int j = 0; j < 9; ++j){
            if(possibilityGrid(i, j) != 0){
                orderedCell.insert(pair<int, pair<int, int>> (possibilityGrid(i, j), pair<int, int>(i, j)));
            }
        }
    }

    return orderedCell;
}
