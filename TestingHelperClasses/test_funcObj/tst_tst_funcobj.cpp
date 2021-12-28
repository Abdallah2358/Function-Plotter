#include <QtWidgets>
#include <QtTest/QtTest>

#include "../../HelperClasses/functionobj.h"
class tst_FuncObjc : public QObject
{
    Q_OBJECT

public:
    tst_FuncObjc();
    ~tst_FuncObjc();
    FunctionObj f;

private slots:
    void validateFunStr_data();
    void validateFunStr();
    void validateRange();
    void splitWithDelimiter_data();
    void splitWithDelimiter();
};
tst_FuncObjc::tst_FuncObjc()
{
}
tst_FuncObjc::~tst_FuncObjc()
{
}
void tst_FuncObjc::validateFunStr_data()
{

    QTest::addColumn<QString>("string");
    QTest::addColumn<int>("result");
    QTest::newRow("Empty Func") << "" << 1;
    QTest::newRow("Test spaces") << "x+ x" << 2;
    QTest::newRow("Non allowed characters") << "x+k" << 3;
    QTest::newRow("Non valid Range") << "x+x" << 4;
}
void tst_FuncObjc::validateFunStr()
{

    QFETCH(QString, string);
    QFETCH(int, result);
    QCOMPARE(f.setFuncStr(string), result);

}
void tst_FuncObjc::validateRange()
{
        f.setFuncStr("x+x");
        f.setSMin(QString("5")); //valid string and valid max non valid max
        QCOMPARE(f.ValidateFunc(), 5);
        f.setSMax(QString("4")); //valid min, max and string but non valid range
        QCOMPARE(f.ValidateFunc(), 6);
        f.setSMax(QString("6")); //pass validation
        QCOMPARE(f.ValidateFunc(), 0);
}

void tst_FuncObjc::splitWithDelimiter_data(){

    QTest::addColumn<QString>("string");
    QTest::addColumn<QStringList>("result");
    QTest::newRow("Empty string") << "" << QStringList({""});
    QTest::newRow("string without delimiter") << "x" << QStringList({"x"});
    QTest::newRow("string containing power") << "x^1" << QStringList({"x^1"});
    QTest::newRow("string with delimiter") << "x+1" << QStringList({"x","+","1"});
    QTest::newRow("string with delimiter containing power") << "x^1+1" << QStringList({"x^1","+","1"});
 }
void tst_FuncObjc::splitWithDelimiter()
{
    QFETCH(QString, string);
    QFETCH(QStringList, result);
    QCOMPARE(f.splitWithDelimiter(string), result);
}

QTEST_MAIN(tst_FuncObjc)
#include "tst_tst_funcobj.moc"
