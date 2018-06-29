#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Account *account) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    account(account)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}
