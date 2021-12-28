#ifndef FUNCTIONOBJ_H
#define FUNCTIONOBJ_H
#include <QDebug>
#include <QRegExp>
#include <QString>
#include <math.h>
#include <vector>
#include <QVBoxLayout>
#include<QGroupBox>
#include<QLabel>

class FunctionObj
{
public:
    FunctionObj();
    FunctionObj(QString inFuncStr, QString inMin, QString inMax , QVBoxLayout * warningLayout =nullptr);
    //validation functions
    bool isValidMin();
    bool isValidMax();
    bool isValidRange();
    int ValidateFunc();
    QStringList splitWithDelimiter();
    QStringList splitWithDelimiter( QString funcStr );
    double handlePower(double x);
    double handlePower( QString str,double x);
    double calculateResult(double x);
    void populateVectors();
    //helper functions
    void reverseQStr(QString &str);
    double operationResult(double op1, double op2, QString op);
    QString createWarning(QString str);
    void showWarning();
    void clearWarning();

    //geters
    double getMin();
    double getMax();
    QVector<double> getX();
    QVector<double> getY();
    //setters
    void setMin(double min);
    void setMax(double max);
    void setSMin(QString min);
    void setSMax(QString max);
    int setFuncStr(QString str);
private:
    void  clearLayout(QLayout *layout);
    QVBoxLayout * warningLayout = nullptr;
    QGroupBox *warningBox = new QGroupBox ("Warning") ;
    QVBoxLayout *vbox = new QVBoxLayout();
    QString funcStr;
    QStringList splitFunctionList;
    QString smin;
    QString smax;
    QVector<double> x ;
    QVector<double> y;
    double min;
    double max;

};

#endif // FUNCTIONOBJ_H
