#ifndef SDKGRID_H
#define SDKGRID_H

#include <vector>
#include <iostream>

class SDKGrid
{
public:

    //init grid to 0
    SDKGrid(std::vector<std::vector<unsigned int>> t_value = {
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0},
    {0, 0, 0 , 0, 0, 0, 0, 0, 0}});

    ~SDKGrid();


    unsigned int &operator ()(unsigned int t_x, unsigned int t_y);

    void reset(int t_x);

    void print();

private:

    size_t m_size;

    std::vector<std::vector<unsigned int>> m_value;

};

#endif // SDKGRID_H
