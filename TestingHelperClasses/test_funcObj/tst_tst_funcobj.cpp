#include <QtWidgets>
#include <QtTest/QtTest>

#include"../../HelperClasses/functionobj.h"
class tst_FuncObjc: public QObject
{
    Q_OBJECT

private slots:

    void validate();
    void splitWithDelimiter();
};

void tst_FuncObjc::validate()
{
    FunctionObj f;
    f.setFuncStr("");//empty func
    QCOMPARE(f.ValidateFunc(), 1);
    f.setFuncStr("x+ x");//there is spaces
    QCOMPARE(f.ValidateFunc(), 2);
    f.setFuncStr("x+x+l");//elemets other than allowed
    QCOMPARE(f.ValidateFunc(), 3);
    f.setFuncStr("x+x");//valid string non valid min
    QCOMPARE(f.ValidateFunc(),4 );
    f.setSMin(QString("5"));//valid string and valid max non valid max
    QCOMPARE(f.ValidateFunc(), 5);
    f.setSMax(QString("4"));//valid min, max and string but non valid range
    QCOMPARE(f.ValidateFunc(), 6);
    f.setSMax(QString("6"));//pass validation
    QCOMPARE(f.ValidateFunc(), 0);

}

void   tst_FuncObjc::splitWithDelimiter(){

}


QTEST_MAIN(tst_FuncObjc)
#include "tst_tst_funcobj.moc"
