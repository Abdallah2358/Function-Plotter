#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../HelperClasses/functionobj.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_plotBtn_clicked()
{
    FunctionObj func(ui->func->text(), ui->min->text(), ui->max->text(), ui->warnLayout);
    if (func.ValidateFunc() == 0)
    {
        func.populateVectors();
        plot = new Plot(func.getX(), func.getY(), this);
        plot->show();
    }
}
void MainWindow::clearVBoxLayout(QVBoxLayout *Layout)
{
    QLayoutItem *item;
    while ((item = Layout->takeAt(0)))
        delete item;
}
