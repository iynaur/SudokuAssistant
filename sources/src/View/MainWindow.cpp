/**
 * Copyright © Maxime Stevenot, Guillaume Hannes
 *
 * This file is part of Sudoku Assistant
 *
 * This software is governed by the CeCILL license under French law
 * and abiding by the rules of distribution of free software.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DigitEntry.h"
#include "VictoryWindow.h"
#include <QString>
#include <QPoint>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

namespace SudokuAssistant {
namespace View {

using namespace Logic;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initComboBox();
    _controller = new Controller();
    _controller->setDifficulty();

    ui->_sudokuBoard->initializeWidget(_controller);

    connect(ui->action_Quit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->action_New, SIGNAL(triggered(bool)), this, SLOT(onNewGame()));
    connect(ui->action_Restart, SIGNAL(triggered(bool)), this, SLOT(onRestartGame()));
    connect(ui->action_Open, SIGNAL(triggered(bool)), this, SLOT(onLoadGame()));
    connect(ui->action_Save, SIGNAL(triggered(bool)), this, SLOT(onSaveGame()));
    connect(ui->action_Save_As, SIGNAL(triggered(bool)), this, SLOT(onSaveGameAs()));
    connect(ui->action_Check_grid, SIGNAL(triggered(bool)), this, SLOT(onCheckGrid()));
    connect(ui->_validateButton, SIGNAL(clicked(bool)), this, SLOT(onCheckGrid()));
    connect(ui->actionShow_Hint, SIGNAL(triggered(bool)), _controller, SLOT(giveHint()));
    connect(ui->_sudokuBoard, SIGNAL(boxClicked(int,int)), this, SLOT(onBoxUpdateRequested(int,int)));
    connect(ui->_hintButton, SIGNAL(clicked(bool)), _controller, SLOT(giveHint()));
    connect(ui->newGameButton, SIGNAL(clicked(bool)), this, SLOT(onNewGame()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(onShowAboutDialog()));
    connect(ui->difficultyComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), _controller, [=](int i){ _controller->setDifficulty(static_cast<Controller::Difficulty>(i)); });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _controller;
}

void MainWindow::initComboBox()
{
    const QString Difficulty_Level[] = { tr("Easy"), tr("Medium"), tr("Hard"), tr("Insane") };

    for (int i = 0; i < Controller::Difficulty_Count; i++)
    {
        ui->difficultyComboBox->addItem(Difficulty_Level[i], static_cast<Controller::Difficulty>(i));
    }
    ui->difficultyComboBox->setCurrentIndex(Controller::Difficulty_Medium);
}

void MainWindow::onBoxUpdateRequested(int i, int j)
{
    View::DigitEntry userInput(i, j, _controller);
    connect(&userInput, SIGNAL(boxUpdated(int,int,int)), _controller, SLOT(onGridUpdate(int,int,int)));

    userInput.move(QCursor::pos());
    userInput.exec();
}

void MainWindow::onSaveGame()
{
    if (_savingPath.isEmpty())
    {
        onSaveGameAs();
    }
    else
    {
        _controller->saveGame(_savingPath);
    }
}

void MainWindow::onSaveGameAs()
{
    QString tempPath = QFileDialog::getSaveFileName(this, QString(), QString(),
                                                    tr("Sudoku (*.sds);;All Files(*)"));

    if (!tempPath.isEmpty())
    {
        _savingPath = tempPath;
        _controller->saveGame(_savingPath);
    }
}

void MainWindow::onLoadGame()
{
    if (_controller->userShouldSave() && !askSaving())
    {
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, QString(), QString(),
                                                tr("Sudoku (*.sds);;All Files(*)"));
    _controller->loadGame(path);
    _savingPath = path;
}

void MainWindow::onNewGame()
{
    if (_controller->userShouldSave() && !askSaving())
    {
        return;
    }
    _controller->onNewGrid();
}

void MainWindow::onCheckGrid()
{
    if (_controller->checkGrid())
    {
        VictoryWindow(this).exec();
    }
}

void MainWindow::closeEvent(QCloseEvent * evt)
{
    if (_controller->userShouldSave() && !askSaving())
    {
        evt->ignore();
        return;
    }
    evt->accept();
}

void MainWindow::onShowAboutDialog()
{
    QMessageBox::about(this,
                       tr("About"),
                       tr("Sudoku Assistant\nCopyright (C) ENSICAEN 2016-2017\nGuillaume Hannes, Maxime Stevenot"));
}

void MainWindow::onRestartGame()
{
    if (_controller->userShouldSave() && !askSaving())
    {
        return;
    }
    _controller->onClearGrid();
}

bool MainWindow::askSaving()
{
    QMessageBox msgBox(this);
    msgBox.setText(tr("The board has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int choice = msgBox.exec();
    bool ret = true;

    switch (choice)
    {
    case QMessageBox::Save :
        onSaveGame();
        break;
    case QMessageBox::Cancel :
        ret = false;
        break;
    default:
        break;
    }

    return ret;
}

}
}
