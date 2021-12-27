#include "plot.h"
#include "ui_plot.h"

Plot::Plot(QVector<double> x, QVector<double> y,QWidget *parent) : QDialog(parent),ui(new Ui::Plot)
{
  ui->setupUi(this);
  this->x  = x;
  this->y = y;
  Plot ::makePlot();
}

Plot::~Plot()
{
  delete ui;
}

void Plot ::makePlot()
{
    double yStart= -y.last();
    double yEnd = y.last();
  // create graph and assign data to it:
  ui->customPlot->addGraph();
  ui->customPlot->graph(0)->setData(x, y);
  // give the axes some labels:
  ui->customPlot->xAxis->setLabel("x");
  ui->customPlot->yAxis->setLabel("f(x)");
  // set axes ranges, so we see all data:
  ui->customPlot->xAxis->setRange(x.first(), x.last());
  for (double point : y ) {
      if(point<yStart)
          yStart = point;
      if(point>yEnd)
          yEnd=point;
  }
  qDebug()<<"y first"<<y.first()<<"y last "<<y.last();
  qDebug()<<"start "<<yStart<<"end "<<yEnd    ;
  ui->customPlot->yAxis->setRange(yStart, yEnd);
  ui->customPlot->replot();
}
