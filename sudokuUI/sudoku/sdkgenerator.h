#ifndef SDKGENERATOR_H
#define SDKGENERATOR_H

#include "sdksolver.h"
#include <ctime>
#include <tuple>

class SDKGenerator
{
public:

    SDKGenerator();

    //return a pair with the grid and its solution
    std::pair<SDKGrid, SDKGrid> create();

    //set abort signal
    void abort();

    //set the number of cell we have to keep the value
    bool setRemainingNb(unsigned int t_value);

    //return generating progress (in percent)
    float getProgress();

private:

    void fillDiagonal();
    void fillBox(unsigned int t_i, unsigned int t_j);


    SDKGrid m_grid;

    bool m_aborted = false;

    unsigned int m_remainingNb = 20;

    float m_progress = 0;
};

#endif // SDKGENERATOR_H
