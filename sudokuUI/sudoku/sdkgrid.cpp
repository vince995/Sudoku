#include "sdkgrid.h"

using namespace std;

SDKGrid::SDKGrid(std::vector<std::vector<unsigned int> > t_value)
{
    m_size = 9;
    m_value.resize(m_size);
    for(size_t i = 0; i < m_size; ++i){
        m_value[i].resize(m_size);
    }

    m_value = t_value;
}

SDKGrid::~SDKGrid()
{

}

unsigned int& SDKGrid::operator ()(unsigned int t_x, unsigned int t_y)
{
    return m_value[t_y][t_x];
}

void SDKGrid::reset(int t_x)
{
    for(size_t i = 0; i < m_size; ++i){
        for(size_t j = 0; j < m_size; ++j){
                m_value[i][j] = t_x;
        }
    }
}

void SDKGrid::print()
{
    for(size_t i = 0; i < m_size; ++i){
        for(size_t j = 0; j < m_size; ++j){
                cout << m_value[i][j] << " ";
                if((j+1)%3 == 0){
                    cout << "| ";
                }
        }
        cout << endl;
        if((i+1)%3 == 0){
            cout << "_______________________" << endl;
        }

    }
    cout << endl;
}
