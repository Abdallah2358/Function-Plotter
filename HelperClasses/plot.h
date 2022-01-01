#ifndef PLOT_H
#define PLOT_H

#include <QDialog>

namespace Ui
{
    class Plot;
}

class Plot : public QDialog
{
    Q_OBJECT

public:
    explicit Plot(QVector<double> x, QVector<double> y, QWidget *parent = nullptr);
    ~Plot();
private slots:
    void makePlot();

private:
    QVector<double> x;
    QVector<double> y;
    Ui::Plot *ui;
};

#endif // PLOT_H
