/*
 * Copyright (C) ENSICAEN 2016-2017
 * Authors : Maxime Stevenot, Guillaume Hannes
 *
 * This file is part of Sudoku Assistant
 *
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <QObject>
#include <QList>
#include <QVector>
#include "Grid.h"

namespace SudokuAssistant {

class Solver : public QObject
{
    Q_OBJECT
public:
    Solver();
    Solver(Grid *);
    ~Solver();
    void changeGrid(Grid *);

    bool CheckGrid();
    bool solve();
    int getCorrectValue(int, int);

signals:
    void incorrectValue(int, int);

private:
    const static int SIZE = 9;
    Grid * _grid;

    struct Pos {
        int x;
        int y;
    };

    struct Cell {
        QList<int> possibleValues = {1,2,3,4,5,6,7,8,9};
        bool assigned = false;
        QList<Pos> peers;
        QList<Pos> units[3];
    };
    QVector<Cell> * _solvedTable;
    int index(int, int);

    void initSolvedTable();

    // Forward Checking
    QVector<Cell> * eliminateFC(QVector<Cell> *, Pos &, int);
    QVector<Cell> * assignFC(QVector<Cell> *, Pos &, int);

    QVector<Cell> * backtrackingSearch(QVector<Cell> *);

    // Minimum Remaining Value + Max Degree
    Pos heuristic(QVector<Cell> *);
    QList<Pos> minimumRemainingValuesList(QVector<Cell> *);
    Pos maxDegree(QVector<Cell> *, QList<Pos>);

    int leastConstraintValue(QVector<Cell> *, Pos);

    // Utils
    bool isFinish(QVector<Cell> *);
    QVector<Cell> * clone(QVector<Cell> * source);
    int getMinIndex(QVector<int> indexesArray);

};

}

#endif // SOLVER_H
