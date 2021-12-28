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
    //re

    //QGroupBox *groupBox = new QGroupBox(tr("Warning"));
    //QVBoxLayout *vbox = new QVBoxLayout();
    //groupBox->setStyleSheet(QString("QGroupBox{border:2px solid gray; color : red ;border-radius:5px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}"));
    //vbox->addWidget(new QLabel(tr("danger")) );
    // groupBox->setLayout(vbox);
    //ui->warnLayout->addWidget(groupBox);
    qDebug() << "func = " << ui->func->text();
    qDebug() << "min = " << ui->min->text();
    qDebug() << "max = " << ui->max->text();
    FunctionObj func(ui->func->text(), ui->min->text(), ui->max->text(), ui->warnLayout);
    //  func.warnUser("stop at once");
    qDebug() << "here";
    if (func.ValidateFunc() == 0)
    {
        //func.populateVectors();
        func.populateVectorsV2();
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
