#ifndef SDKSOLVER_H
#define SDKSOLVER_H

#include "sdkgrid.h"
#include <map>
#include <limits>

struct pos {
    unsigned int x;
    unsigned int y;
};

class SDKSolver
{
public:
    SDKSolver();

    //solve the given grid and return the number of solution if t_countSolution is true and return 1 else
    unsigned int solve(SDKGrid &t_grid, bool t_countSolution = false);

    //check if a grid has multiple solution using the countSOlution function
    bool multipleSolution(SDKGrid t_grid);

    //check if a number is valid
    static bool isValid(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j);



private:




    //check if a value is on the row, column or block
    static bool isOnTheRow(unsigned int t_value, SDKGrid t_grid, unsigned int t_row);
    static bool isOnTheColumn(unsigned int t_value, SDKGrid t_grid, unsigned int t_column);
    static bool isOnTheBlock(unsigned int t_value, SDKGrid t_grid, unsigned int t_i, unsigned int t_j);

    //recursive function to fill a grid
    bool fill(SDKGrid &t_grid, std::multimap<int, std::pair<int, int>>::iterator t_pos);

    //recursive function to count solutions of a grid
    bool countSolution(SDKGrid t_grid, std::multimap<int, std::pair<int, int>>::iterator t_pos, unsigned int &t_count, unsigned int t_limite = std::numeric_limits<unsigned int>::max());

    //order cells (but doesn't work for the moment
    std::multimap<int, std::pair<int, int> > orderCell(SDKGrid t_grid);

    //map to stock cells value and position
    std::multimap<int, std::pair<int, int>> m_orderedCell;

};

#endif // SDKSOLVER_H
