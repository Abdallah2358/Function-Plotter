#include "functionobj.h"

FunctionObj::FunctionObj()
{
}

FunctionObj::FunctionObj(QString inFuncStr, QString inMin, QString inMax, QVBoxLayout *warningLayout)
{
    this->funcStr = inFuncStr;
    this->smax = inMax;
    this->smin = inMin;
    this->warningLayout = warningLayout;

    // style waring box
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
        createWarning("The Function can not be empty");
        showWarning();
        return 1; 
    }
    if (funcStr.contains(" "))
    {
        createWarning("The Function can not contain spaces");
        showWarning();
        return 2;
    }
    QRegExp checkInputExp("[\\dx+\\-\\*/^.]*");
    if (!checkInputExp.exactMatch(funcStr))
    {
        createWarning("The Function can only contain numbers, \"x\"and this operators + - / * ^");
        showWarning();
        return 3;
    }

    if (funcStr.contains("/0"))
    {
        createWarning("You can not divide by 0");
        showWarning();
        return 7;
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
        return op1 / op2;
    if (op == "*")
        return op1 * op2;
    return 0;
}

double FunctionObj::calculateResult(QStringList list, double x)
{
    QStringList::iterator it = list.begin();
    QVector<double> numVector;
    QVector<QString> operatorVector;
    bool skipFlag = false;
    bool isTempDouble = false;
    double dtemp;
    while (it != list.end())
    {
        //iterate over given list
        QString tempStr = *it;
        dtemp = tempStr.toDouble(&isTempDouble);
        //do I skip?
        if (skipFlag)
        {
            it++;
            continue;
        }
        if (isTempDouble)
        {
            //is it a number
            numVector.append(dtemp);
        }// it is not a number
        else if (tempStr == "x")
        {   
            //is it an "x"
            numVector.append(x);
        }// it is not an x then it is an operator 
        else if (tempStr.contains("^"))
        {
            // if power then handle it 
            numVector.append(handlePower(tempStr, x));
        }
        else if (tempStr == "+" || tempStr == "-")
        {
            //is it addition or subtraction operator
            //then put it in operators stack
            operatorVector.append(tempStr);
        }
        else
        {
            // then only left option is multiplication or division
            skipFlag = true; //skip converting next number
            //since multiplication or division should be done from left to right
            QString nextStr = (*(it + 1)); //store next number or x or power number
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
            numVector.append(operationResult(op1, op2, *it));
        }
        it++;
    } //now after replacing "x" , handling power , multiplication and division
    // we handle addition and subtraction
    while (!operatorVector.empty())
    {
        double op2 = numVector.takeLast();
        double op1 = numVector.takeLast();
        numVector.append(operationResult(op1, op2, operatorVector.takeLast()));
    }
    //now we have our result at the top of the stack
    return numVector.last();
}


void FunctionObj::populateVectors()
{
    for (double j = min; j <= max; j += ((max - min) / 100))
    {
        // put point in x vector
        x.append(j);
        //put result in y vector
        y.append(
            //calculate result from received list 
            calculateResult(
                //split the received text to be processed 
                splitWithDelimiter(funcStr), j)
                );
    }
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

QStringList FunctionObj::splitWithDelimiter(QString funcStr)
{
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
    return queryWithSeparators;
}

QString FunctionObj::createWarning(QString str)
{
    if (warningLayout != nullptr)
    {
        vbox->addWidget(new QLabel(str));
        return str;
    }
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
