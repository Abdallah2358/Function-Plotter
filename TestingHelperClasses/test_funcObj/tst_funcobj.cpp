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
    void handlePower_data();
    void handlePower();

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
    QTest::newRow("divide number by 0") << "1/0" << 7;
    QTest::newRow("divide floating point by 0") << "1.1/0" << 7;
     QTest::newRow("divide powered numer") << "1.1^2/0" << 7;
    QTest::newRow("divide x by 0") << "x/0" << 7;
     QTest::newRow("divide x^somthing by 0") << "x^2/0" << 7;
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

void tst_FuncObjc::handlePower_data(){
    QTest::addColumn<QString>("string");
    QTest::addColumn<double>("x");
    QTest::addColumn<double>("result");
    QTest::newRow("Empty string") << "" << 0.0 << 0.0;
    QTest::newRow("1 level of power") << "5^2" << 0.0 << pow(5,2);
    QTest::newRow("2 level of power") << "2^2^2" << 0.0 << pow(2,pow(2,2));
    QTest::newRow("3 level of power") << "2^2^2^2" << 0.0 << pow(2,pow(2,pow(2,2)));
    QTest::newRow("1 level of power of negative values") << "-5^2" << 0.0 << pow(-5,2);
    QTest::newRow("1 level of power of negative values") << "5^-2" << 0.0 << pow(5,-2);
    QTest::newRow("1 level of power of floating point") << "5.2^2" << 0.0 << pow(5.2,2);
    QTest::newRow("1 level of power of floating point") << "5^2.2" << 0.0 << pow(5,2.2);
    QTest::newRow("1 level of power of floating point") << "5.2^2.2" << 0.0 << pow(5.2,2.2);
    QTest::newRow("replace x in power") << "x^2" << 3.0 << pow(3,2);
    QTest::newRow("replace x in power") << "2^x" << 3.0 << pow(2,3);
    QTest::newRow("replace x of negative values in power") << "x^2" << -3.0 << pow(-3,2);
    QTest::newRow("replace x of negative values in power") << "2^x" << -3.0 << pow(2,-3);
}

void tst_FuncObjc::handlePower(){
    QFETCH(QString, string);
    QFETCH(double, x);
    QFETCH(double, result);
    QCOMPARE(f.handlePower(string , x), result);
}

QTEST_MAIN(tst_FuncObjc)
#include "tst_tst_funcobj.moc"
