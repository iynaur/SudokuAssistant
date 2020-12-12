/**
 * Copyright © Maxime Stevenot, Guillaume Hannes
 *
 * This file is part of Sudoku Assistant
 *
 * This software is governed by the CeCILL license under French law
 * and abiding by the rules of distribution of free software.
 */

#include "GridLoader.h"
#include <random>
#include <chrono>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <fstream>
#include <sstream>

namespace SudokuAssistant {
namespace Logic {

using namespace Model;

Grid * GridLoader::getNewGrid(Controller::Difficulty difficulty)
{
    QFile * file = openFile(difficulty);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return nullptr;
    }

    QTextStream in(file);
    int lineCount = in.readLine().toInt();
    int lineIndex = getRandomLineIndex(lineCount);

    QStringList line = getLine(in, lineIndex);
    file->close();

    return new Grid(line);
}

Grid * GridLoader::getNewGridFromSave(const QString & path)
{
    Grid * grid = new Grid();

    if (path.isEmpty())
    {
        return nullptr;
    }

    std::ifstream t(path.toStdString());
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto content = buffer.str();
    grid->fromString(content);
    return grid;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return nullptr;
    }

    QDataStream in(&file);
    in >> (*grid);
    file.close();

    return grid;
}

QFile * GridLoader::openFile(Controller::Difficulty difficulty)
{
    QString fileName;

    switch (difficulty)
    {
    case Controller::Difficulty_Easy:
        fileName = ":/grids/Easy";
        break;
    case Controller::Difficulty_Medium:
        fileName = ":/grids/Medium";
        break;
    case Controller::Difficulty_Hard:
        fileName = ":/grids/Hard";
        break;
    case Controller::Difficulty_Insane:
        fileName = ":/grids/Insane";
        break;
    default:
        fileName = ":/grids/Medium";
        break;
    }

    return new QFile(fileName);
}

int GridLoader::getRandomLineIndex(int lineCount)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(1, lineCount);
    return distribution(generator);
}

QStringList GridLoader::getLine(QTextStream & in, int index)
{
    int currentLine = 1;
    QString line;

    do
    {
        line = in.readLine();
        currentLine++;
    } while (!in.atEnd() && currentLine < index);

    return line.split(" ");
}

}
}
