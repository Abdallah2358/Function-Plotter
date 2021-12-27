#include <QtTest>

// add necessary includes here
#include "../../HelperClasses/functionobj.h"
class FunctionObj : public QObject
{
    Q_OBJECT

public:
    FunctionObj();
    ~FunctionObj();

private slots:
    void test_case1();

};

FunctionObj::FunctionObj()
{

}

FunctionObj::~FunctionObj()
{
    
}

void FunctionObj::test_case1()
{

}

QTEST_APPLESS_MAIN(FunctionObj)

#include "tst_functionobj.moc"
