/**
 * Copyright © Maxime Stevenot, Guillaume Hannes
 *
 * This file is part of Sudoku Assistant
 *
 * This software is governed by the CeCILL license under French law
 * and abiding by the rules of distribution of free software.
 */

#include "Grid.h"
#include <stdexcept>
#include <sstream>
using namespace std;

namespace SudokuAssistant {
namespace Model {

void Grid::fromString(const string &data)
{
    stringstream ss;
    ss<<data;
    char c = 'x';
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            ss>>c;
            while(c != ' ' && c != '-' && (c < '0' || c > '9') ){
                ss>>c;
            }
            int val = 0;
            if (c > '0' && c <= '9') val = c - '0';
            if (val == 0)
            {
                _tab[i][j].readOnly = false;
            }
            else
            {
                _tab[i][j].readOnly = true;
            }
            _tab[i][j].value = val;
        }
    }
}

Grid::Grid(QStringList initList)
{
    load(initList);
}

void Grid::load(QStringList list)
{
    if (list.size() != SIZE * SIZE)
    {
        throw std::out_of_range("Init list of range");
    }

    int index = 0;

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (list[index] == "0")
            {
                _tab[i][j].readOnly = false;
            }
            else
            {
                _tab[i][j].readOnly = true;
            }
            _tab[i][j].value = list[index].toInt();
            index++;
        }
    }
}

void Grid::clear()
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (! _tab[i][j].readOnly)
            {
                _tab[i][j].value = 0;
            }
        }
    }
}

void Grid::setValue(int i, int j, int value)
{
    if (i < 0 || j < 0 || i >= SIZE || j > SIZE)
    {
        throw std::out_of_range("Grid out of range");
    }

    if (!_tab[i][j].readOnly)
    {
        _tab[i][j].value = value;
        emit onValueChanged(i, j, value);
    }
}

int Grid::getValue(int i, int j) const
{
    if (i < 0 || j < 0 || i >= SIZE || j > SIZE)
    {
        throw std::out_of_range("Grid out of range");
    }

    return _tab[i][j].value;
}

bool Grid::isReadOnly(int i, int j) const
{
    if (i < 0 || j < 0 || i >= SIZE || j > SIZE)
    {
        throw std::out_of_range("Grid out of range");
    }

    return _tab[i][j].readOnly;
}

QDataStream & operator<<(QDataStream & out, const Grid & grid)
{
    for (int i = 0; i < Grid::SIZE; ++i)
    {
        for (int j = 0; j < Grid::SIZE; ++j)
        {
            out << grid._tab[i][j].readOnly;
            out << grid._tab[i][j].value;
        }
    }

    return out;
}

QDataStream & operator>>(QDataStream & in, Grid & grid)
{
    for (int i = 0; i < Grid::SIZE; ++i)
    {
        for (int j = 0; j < Grid::SIZE; ++j)
        {
            in >> grid._tab[i][j].readOnly;
            in >> grid._tab[i][j].value;
        }
    }

    return in;
}

}
}
