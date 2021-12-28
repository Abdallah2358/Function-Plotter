#include "functionobj.h"

FunctionObj::FunctionObj()
{
}

FunctionObj::FunctionObj(QString inFuncStr, QString inMin, QString inMax, QVBoxLayout *warningLayout)
{
    qDebug() << inFuncStr << ", " << inMin << " ," << inMax;
    this->funcStr = inFuncStr;
    this->smax = inMax;
    this->smin = inMin;
    this->warningLayout = warningLayout;
    this->warningBox->setStyleSheet(
        QString(""
                "QGroupBox{border:2px solid gray; color : red ;border-radius:5px;margin-top: 1ex;}"
                " QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}"));
    this->warningBox->setLayout(this->vbox);
}

//validation
int FunctionObj::ValidateFunc()
{
    //remove spaces
    funcStr = funcStr.trimmed();
    if (funcStr.isEmpty())
    {
        //qDebug() << "The Function can not be empty";
        createWarning("The Function can not be empty");
        showWarning();
        return 1; // 1 indicated empty string
        //give message "Empty Input"
    }
    if (funcStr.contains(" "))
    {
        //qDebug() << "The Function can not contain spaces";
        createWarning("The Function can not contain spaces");
        showWarning();
        return 2;
    }
    QRegExp checkInputExp("[\\dx+\\-\\*/^]*");
    if (!checkInputExp.exactMatch(funcStr))
    {
        //qDebug() << "The Function can only contain numbers, \"x\"and this operators + - / * ^";
        createWarning("The Function can only contain numbers, \"x\"and this operators + - / * ^");
        showWarning();
        return 3;
    }
    if (!isValidMin())
    {
        createWarning("please provide a valid decimal number in Min");
        showWarning();
        return 4;
    }
    if (!isValidMax())
    {
        createWarning("please provide a valid decimal number in Max");
        showWarning();
        return 5;
    }
    if (!isValidRange())
    {
        createWarning("please provide a valid range Max > Min");
        showWarning();
        return 6;
    }
    clearWarning();
    //qDebug() << "validation done fun str : " << funcStr;
    return 0;
}
bool FunctionObj::isValidMin()
{
    bool ok;
    this->min = smin.toDouble(&ok);
    return ok;
}
bool FunctionObj::isValidMax()
{
    bool ok;
    this->max = smax.toDouble(&ok);
    if (!ok)
    {
        // qDebug("please enter valid maximum");
    }
    return ok;
}

bool FunctionObj::isValidRange()
{
    if (getMax() > getMin())
        return true;
    //qDebug("please enter valid range, the min can only be less than max");
    return false;
}

//calculation

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
                //iterate over the numbers from right to left stacking each result to the right
                //ex : 2^3^4 is done in two steps
                //3^4 =81
                //2^81
                //then replace the string "2^3^4" in the list by the result
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
double FunctionObj::handlePower(QString str, double x = 1)
{
    /*
   Microsoft Excel and computation programming language MATLAB evaluate a^b^c as (a^b)^c, 
    but Google Search and Wolfram Alpha as a^(b^c).
    Thus 4^3^2 is evaluated to 4,096 in the first case and to 262,144 in the second case. 
    source :    https://en.wikipedia.org/wiki/Order_of_operations (Special cases
    Serial exponentiation   ) 
    */
    QStringList operands = str.split("^");
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
        //iterate over the numbers from right to left stacking each result to the right
        //ex : 2^3^4 is done in two steps
        //3^4 =81
        //2^81
        //then replace the string "2^3^4" in the list by the result
        //qDebug() <<"it = "<< *it <<" ,res= "<<res;
        bool isTempDouble = true;
        double temp = (*tempit).toDouble(&isTempDouble);
        if (!isTempDouble)
            temp = x;
        // qDebug() << "temp = " << temp << " ,res= " << res;
        res = pow(temp, res);
        tempit--;
    }
    return res;
}
double FunctionObj::operationResult(double op1, double op2, QString op)
{
    if (op == "+")
        return op1 + op2;
    if (op == "-")
        return op1 - op2;
    if (op == "/")
    {
        return op1 / op2;
    }
    if (op == "*")
        return op1 * op2;
    return 0;
}

double FunctionObj::calculateResult(double x)
{

    QStringList::iterator it = splitFunctionList.begin();
    QVector<double> numVector;
    QVector<QString> operatorVector;
    bool skipFlag = false;
    qDebug() << "start of calclate of x = " << x;
    while (it != splitFunctionList.end())
    {
        bool isNegative = false;

        //qDebug() << "checking x";
        if ((*it) == "x")
            *it = QString::number(x);
        // qDebug() <<" done checking x";
        (*it).toDouble(&isNegative);
        if (!isNegative && (*it).contains(QRegExp("[+\\-\\*\\/]")))
        {
            qDebug() << isNegative;
            if (*it == "+" || *it == "-")
            {
                qDebug() << "operator appended : " << *it << "  ,1";
                operatorVector.append(*it);
            }
            else if ((*it).contains("e"))
            {
                numVector.append((*it).toDouble());
                qDebug() << "skiped  :" << *it;
            }
            else
            {
                skipFlag = true;
                if (operatorVector.empty())
                {
                    if (*(it + 1) == "x")
                        *(it + 1) = QString::number(x);

                    qDebug() << numVector.last() << *it << (*(it + 1)).toDouble() << "  ,2";
                    numVector.append(operationResult(numVector.takeLast(), (*(it + 1)).toDouble(), *it));
                }
                else
                {
                    if (*(it + 1) == "x")
                        *(it + 1) = QString::number(x);
                    qDebug() << numVector.last() << *it << (*(it + 1)).toDouble() << "  ,3";
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
    qDebug() << "done appending";
    while (!operatorVector.empty())
    {
        //qDebug() <<"in while appending";
        double op2 = numVector.takeLast();
        double op1 = numVector.takeLast();
        qDebug() << op1 << operatorVector.last() << op2 << " = " << operationResult(op1, op2, operatorVector.last()) << "  ,4";
        numVector.append(operationResult(op1, op2, operatorVector.takeLast()));
    }
    qDebug() << "result = " << numVector.last();
    return numVector.last();
}

double FunctionObj::calculateResult(QStringList list, double x)
{
 qDebug() << "start of calclate of x = " << x;
    QStringList::iterator it = list.begin();
    QVector<double> numVector;
    QVector<QString> operatorVector;
    bool skipFlag = false;

    bool isTempDouble = false;
    double dtemp;
    while (it != splitFunctionList.end())
    {

        QString tempStr = *it;
        dtemp = tempStr.toDouble(&isTempDouble);
        if (!skipFlag) {
            if (isTempDouble)
            {
                //is it a number
                numVector.append(dtemp);
                continue;
            }
            else if (tempStr == "x")
            {
                //is it an "x"
                numVector.append(x);
                continue;
            }
            else if (tempStr.contains("^"))
            {
                // is it a power
                numVector.append(handlePower(tempStr, x));
            }
            else if (tempStr == "+" || tempStr == "-")
            {
                //is it a plus or a muins
                //then put it in oprators stack
                numVector.append(handlePower(tempStr, x));
            }
            else
            {
                // then only left option is multiplication or divition

                skipFlag = true; //skip converting next number
                //since multiplication or dividtion should be done from left to right
                QString nextStr = *(it + 1); //store next number or x
                dtemp = nextStr.toDouble(&isTempDouble);
                if (!isTempDouble) // if it is not a number then it must be x or a term with power
                {
                    if (nextStr == "x") // if it is x
                        dtemp = x;
                    else //then it must be a term with power
                        dtemp = handlePower(nextStr, x);
                }
                double op1 = numVector.takeLast();
                double op2 = dtemp;
                qDebug() << op1 << *it << op2 << "  ,2";
                if (op2 == 0)
                {
                    createWarning("your function contains division by 0 which is not allowed");
                    showWarning();
                    numVector.append(0);
                }
                else
                {
                    numVector.append(operationResult(op1, op2, *it));
                }
            }
        }//now after replacing "x" , handling power , multiplication and division

     }


    // we handle addition and subtraction
    while (!operatorVector.empty())
    {
        //qDebug() <<"in while appending";
        double op2 = numVector.takeLast();
        double op1 = numVector.takeLast();
        qDebug() << op1 << operatorVector.last() << op2 << " = " << operationResult(op1, op2, operatorVector.last()) << "  ,4";
        numVector.append(operationResult(op1, op2, operatorVector.takeLast()));
    }
    //now we have our result at the top of the stack
    qDebug() << "result = " << numVector.last();
    return numVector.last();
}

void FunctionObj::populateVectors()
{
    QString temp = funcStr;
    for (double j = min; j <= max; j += ((max - min) / 100))
    {
        splitWithDelimiter();
        x.append(j);
        y.append(calculateResult(j));
        funcStr = temp;
    }
    qDebug() << "populate vector done";
}


void FunctionObj::populateVectorsV2()
{
    for (double j = min; j <= max; j += ((max - min) / 100))
    {
        x.append(j);
        y.append(calculateResult(splitWithDelimiter(funcStr),j));
    }
    qDebug() << "populate vector v2 done";
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
void FunctionObj::setSMin(QString min)
{
    this->smin = min;
}
void FunctionObj::setSMax(QString max)
{
    this->smax = max;
}
int FunctionObj::setFuncStr(QString str)
{
    this->funcStr = str;
    return ValidateFunc();
}
// helper functions

QStringList FunctionObj::splitWithDelimiter()
{
    //QString funcStr = "" , QRegExp rsx =QRegExp("")
    QRegExp rx("([+\\-\\*\\/])");
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
    return queryWithSeparators;
}

QStringList FunctionObj::splitWithDelimiter(QString funcStr)
{
    //QString funcStr = "" , QRegExp rsx =QRegExp("")
    QRegExp rx("([+\\-\\*\\/])");
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
    //splitFunctionList = queryWithSeparators;
    return queryWithSeparators;
}

//separting this functions allows me to add multiple warning in the same box
QString FunctionObj::createWarning(QString str)
{
    if (warningLayout != nullptr)
    {
        vbox->addWidget(new QLabel(str));
        qDebug() << str;
        return str;
    }
    //qDebug() << "please provide layout";
    return "please provide layout";
}
void FunctionObj::showWarning()
{
    if (warningLayout != nullptr)
    {
        clearWarning();
        warningLayout->addWidget(warningBox);
    }
}
void FunctionObj::clearWarning()
{
    if (warningLayout != nullptr)
    {
        clearLayout(warningLayout);
    }
}
void FunctionObj::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    QLayout *sublayout;
    QWidget *widget;

    while ((item = layout->takeAt(0)))
    {
        if ((sublayout = item->layout()) != 0)
        {
            clearLayout(sublayout);
        }
        else if ((widget = item->widget()) != 0)
        {
            widget->hide();
            delete widget;
        }
        else
        {
            delete item;
            return;
        }
    }
    return;
}
