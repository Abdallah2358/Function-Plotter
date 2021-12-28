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
    if (!ok)
    {
        //qDebug("please enter valid minmum");
    }
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

double FunctionObj::operationResult(double op1, double op2, QString op)
{
    if (op == "+")
        return op1 + op2;
    if (op == "-")
        return op1 - op2;
    if (op == "/")
    {
        qDebug()<<"op2 in division" << (op2==0);
        if (op2 == 0)
        {
            qDebug()<<"IN IF in division 1" ;
            createWarning("You Can`t divide by 0 please check your Function");
           // createWarning("All Results on the plot Can not be trusted");
             qDebug()<<"IN IF in division 2" ;
            showWarning();
             qDebug()<<"IN IF in division 3" ;
            return 1;
        }
        return op1 / op2;
    }
    if (op == "*")
        return op1 * op2;
    return 0;
}

double FunctionObj::calculateResult(double x  )
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


void FunctionObj::populateVectors()
{
    QString temp =funcStr;
    for (double j = min; j <= max; j += ((max - min) / 100))
    {
        splitWithDelimiter();
        handlePower(j);
        x.append(j);
        y.append(calculateResult(j ));
        funcStr = temp;
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
void FunctionObj::setSMin(QString min)
{
    this->smin = min;
}
void FunctionObj::setSMax(QString max)
{
    this->smax = max;
}
void FunctionObj::setFuncStr(QString str){
    this->funcStr =str;
}
// helper functions

QStringList FunctionObj::splitWithDelimiter( )
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
    return queryWithSeparators  ;
}


QStringList FunctionObj::splitWithDelimiter( QString funcStr  )
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
    return queryWithSeparators  ;
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
    if (warningLayout != nullptr) {
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
