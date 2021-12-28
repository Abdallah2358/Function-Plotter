#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include "../HelperClasses/plot.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //   bool isDigit(QString str);
private slots:
    void on_plotBtn_clicked();

private:
    Ui::MainWindow *ui;
    Plot *plot;
    void clearVBoxLayout(QVBoxLayout *vbox);
};
#endif // MAINWINDOW_H
