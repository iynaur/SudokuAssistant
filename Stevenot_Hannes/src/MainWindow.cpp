#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(NULL));

    initComboBox();

    _controller = new Controller();
    _controller->setDifficulty();

    ui->_sudokuBoard->initWidget(_controller);

    connect(ui->action_New, SIGNAL(triggered(bool)), _controller, SLOT(newGrid()));
    connect(ui->action_Quit, SIGNAL(triggered(bool)), this, SLOT(exitApplication()));
    connect(ui->newGameButton, SIGNAL(released()), _controller, SLOT(newGrid()));
    connect(ui->difficultyComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), _controller, [=](int i){ _controller->setDifficulty(static_cast<Controller::Difficulty>(i)); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initComboBox()
{
    for (int diff = 0; diff < Controller::Difficulty_Count; diff++)
    {
        ui->difficultyComboBox->addItem(Controller::Difficulty_Level[diff], static_cast<Controller::Difficulty>(diff));
    }
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}
