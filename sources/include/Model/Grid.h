/**
 * Copyright © Maxime Stevenot, Guillaume Hannes
 *
 * This file is part of Sudoku Assistant
 *
 * This software is governed by the CeCILL license under French law
 * and abiding by the rules of distribution of free software.
 */

#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QDataStream>

namespace SudokuAssistant {
namespace Model {

class Grid : QObject
{
    Q_OBJECT

    friend QDataStream & operator<<(QDataStream & out, const Grid &);
    friend QDataStream & operator>>(QDataStream & in, Grid &);


public:
    void fromString(const std::string &data);
    const static int SIZE = 9;

    Grid() = default;
    Grid(QStringList);

    int getValue(int i, int j) const;
    bool isReadOnly(int i, int j) const;

signals:
    void onValueChanged(int i, int j, int value);

public slots:
    void setValue(int i, int j, int value);
    void load(QStringList);
    void clear();

private:
    struct Cell {
        int value;
        bool readOnly;
    };

    Cell _tab[SIZE][SIZE];

};

QDataStream & operator<<(QDataStream & out, const Grid &);
QDataStream & operator>>(QDataStream & in, Grid &);

}
}

#endif
