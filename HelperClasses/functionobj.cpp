#include "functionobj.h"

FunctionObj::FunctionObj()
{
}

FunctionObj::FunctionObj(QString inFuncStr, QString inMin, QString inMax)
{
    qDebug() << inFuncStr << ", " << inMin << " ," << inMax;
    this->funcStr = inFuncStr;
    this->smax = inMax;
    this->smin = inMin;

}

int FunctionObj::ValidateFunc()
{
    //remove spaces
    funcStr = funcStr.trimmed();
    if (funcStr.isEmpty())
    {
        qDebug() << "The Function can not be empty";
        return 1; // 1 indicated empty string
        //give message "Empty Input"
    }

    if (funcStr.contains(" "))
    {
        qDebug() << "The Function can not contain spaces";
        return 2;
    }
    QRegExp checkInputExp("[\\dx+\\-\\*/^]*");
    if (!checkInputExp.exactMatch(funcStr))
    {
        qDebug() << "The Function can only contain numbers, \"x\"and this operators + - / * ^";
        qDebug() << "fun str : " << funcStr;
        return 3;
    }
    if(!isValidMin())
        return 4;
    if(!isValidMax())
        return 5;
    if(!isValidRange())
        return 6;
    qDebug() << "validation done fun str : " << funcStr;
    return 0;
}
double FunctionObj::handlePower(double x)
{
    QStringList::iterator it = splitFunctionList.begin();
    while (it != splitFunctionList.end())
    {
        if ((*it).contains("^"))
        {
            QStringList operands = (*it).split("^");
            QStringList::iterator tempit = operands.end() - 1;
            //if it is not a double then it should be x since we did our validation
            bool isResDouble = true;
            double res = (*tempit).toDouble(&isResDouble);
            if (!isResDouble)
                res = x;
            tempit--;
            //go to next item
            while (tempit != operands.begin() - 1)
            {
                //qDebug() <<"it = "<< *it <<" ,res= "<<res;
                bool isTempDouble = true;
                double temp = (*tempit).toDouble(&isTempDouble);
                if (!isTempDouble)
                    temp = x;
                // qDebug() << "temp = " << temp << " ,res= " << res;
                res = pow(temp, res);
                tempit--;
            }
            (*it) = QString::number(res);
        }
        it++;
    }
    qDebug() << "handle power done";
    return 0;
}
bool FunctionObj::isValidMin()
{
    bool ok;
    min = smin.toDouble(&ok);
    if (!ok)
        qDebug("please enter valid minmum");
    return ok;
}
bool FunctionObj::isValidMax()
{
    bool ok;
    max = smax.toDouble(&ok);
    if (!ok)
        qDebug("please enter valid maximum");
    return ok;
}
bool FunctionObj::isValidRange()
{
    if (getMax() > getMin())
        return true;
    qDebug("please enter valid range, the min can only be less than max");
    return false;
}

double FunctionObj::calculateResult(double x)
{


    QStringList::iterator it = splitFunctionList.begin();

    QVector<double> numVector;
    QVector<QString> operatorVector;
    bool skipFlag = false;
    qDebug() << "start of calclate of x = "<<x;
    while (it != splitFunctionList.end())
    {
        bool isNegative = false;

        //qDebug() << "checking x";
        if ((*it) == "x")
            *it = QString::number(x);
        // qDebug() <<" done checking x";
        (*it).toDouble(&isNegative);
        if ( !isNegative &&(*it).contains(QRegExp("[+\\-\\*\\/]")) )
        {
              qDebug() <<isNegative;
            if (*it == "+" || *it == "-")
            {
               qDebug() << "operator appended : "<< *it<<"  ,1";
                operatorVector.append(*it);
            }
            else  if ((*it).contains("e")) {
                numVector.append((*it).toDouble());
                qDebug() << "skiped  :" << *it;
            }else
            {
                skipFlag = true;
                if (operatorVector.empty())
                {
                    if (*(it + 1) == "x")
                        *(it + 1) = QString::number(x);

                    qDebug() << numVector.last() << *it << (*(it + 1)).toDouble() <<"  ,2";
                    numVector.append(operationResult(numVector.takeLast(), (*(it + 1)).toDouble(), *it));
                }
                else
                {
                    if (*(it + 1) == "x")
                        *(it + 1) = QString::number(x);
                    qDebug() << numVector.last() << *it << (*(it + 1)).toDouble()<<"  ,3";
                    numVector.append(operationResult(numVector.takeLast(), (*(it + 1)).toDouble(), *it));
                }
            }
            qDebug() << "the operator" << *it;
        }
        else
        {
            if (!skipFlag)
            {
                //check if any number exceds the double max
                if ((*it).toDouble() > std::numeric_limits<double>::max())
                {
                    qDebug() << "This program cant handle numbers more than : " << std::numeric_limits<double>::max();
                    return 1; //mea
                }
                numVector.append((*it).toDouble());
                qDebug() << "string in func" << *it << "its converstion" << numVector.last();
               // qDebug() << "vec size" << numVector.size();
            }
            skipFlag = false;
        }
        ++it;
    }
  qDebug() <<"done appending";
    while (!operatorVector.empty())
    {
        //qDebug() <<"in while appending";
        double op2 = numVector.takeLast();
        double op1 = numVector.takeLast();
        qDebug() << op1 << operatorVector.last() <<op2 <<" = "<<operationResult(op1, op2, operatorVector.last())<<"  ,4";
        numVector.append(operationResult(op1, op2, operatorVector.takeLast()));
    }
    qDebug() << "result = " << numVector.last();
    return numVector.last();
}

void FunctionObj::populateVectors()
{

    QString temp = QString(funcStr);
    for (double j = min; j <= max; j += ((max - min) / 100))
    {
        splitFunction();
        handlePower(j);
        x.append(j);
        y.append(calculateResult(j));
        funcStr=QString(temp);
    }

    qDebug() << "populate vector done";
}

//getters
QVector<double> FunctionObj::getX() { return x; }

QVector<double> FunctionObj::getY() { return y; }

double FunctionObj::getMin() { return min; }

double FunctionObj::getMax() { return max; }

//setters
void FunctionObj::setMin(double min)
{
    this->min = min;
}
void FunctionObj::setMax(double max)
{
    this->max = max;
}

// helper functions
void FunctionObj::reverseQStr(QString &str)
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
    {
        QChar temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}
void FunctionObj::splitFunction()
{
    QRegExp rx("[+\\-\\*\\/]");
    QStringList query = funcStr.split(rx);
    QStringList queryWithSeparators;
    int strLen = funcStr.length();
    int pos = 0;
    for (const QString part : query)
    {
        queryWithSeparators.append(part);
        pos += part.length();
        if (pos + 1 < strLen)
        {
            // we know that the separators are all 1 character long
            queryWithSeparators.append(QString(funcStr[pos]));
            pos += 1;
        }
    }
    splitFunctionList = queryWithSeparators;
}
double FunctionObj::operationResult(double op1, double op2, QString op)
{
    if (op == "+")
        return op1 + op2;
    if (op == "-")
        return op1 - op2;
    if (op == "/")
        return op1 / op2;
    if (op == "*")
        return op1 * op2;
    return 0;
}
