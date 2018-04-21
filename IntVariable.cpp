#include "IntVariable.h"
#include <iostream>
using namespace std;


/*
 * constructor of new int variable.
 * value - the given value of the variable.
 * name - the given name of the variable.
 * type - the type of the variable
 */
IntVariable::IntVariable(int newVal, string name, string type) : Variable(name,type)
{
    m_intValue = newVal;
}

/*
 *destructor of the variable.
 */
IntVariable::~IntVariable()  {}


/*
 * return the value of the variable.
 */
int IntVariable::getVal()const
{
    return m_intValue;
}

/*
 * sets new value of the current variable.
 */
void IntVariable::setVal(int newVal)
{
    m_intValue = newVal;
}


/*
 * return the sum of the values current variable and a given one.
 * addTo - the given variable that need to be added.
 */
IntVariable& IntVariable::operator+(const IntVariable& addTo)
{
    IntVariable *newInt = new IntVariable(0,"noName","int");
    int tmp1 = addTo.getVal();
    int tmp2 = getVal();
    newInt->setVal(tmp1 + tmp2);
    return *newInt;
}

/*
 *return a given value to the current value.
 * addToPlusOne - the given variable that added.
 */
IntVariable& IntVariable::operator+=(const IntVariable& addToPlusOne)
{
    int tmp = addToPlusOne.getVal();
    setVal(tmp + m_intValue);
    return *this;
}

/*
 * copying a value of a given variable into te current variable.
 * newVal - the given variable.
 */
IntVariable& IntVariable::operator=(const IntVariable& newVal)
{
    int tmp = newVal.getVal();
    setVal(tmp);
    return *this;
}

/*
 * returns true or false is the current variable's value is identical the given variable's value.
 * equalTo - the given var
 */
bool IntVariable::operator==(const IntVariable& equalTo)const
{
    return m_intValue == equalTo.getVal();
}


/*
 * A private function to print the variable.
 */
ostream & IntVariable::Show(ostream & out)const
{
    cout<<m_intValue;
}

/*
 * printing a given variable with the right format
 */
ostream & operator<<(ostream& out,const IntVariable & i)
{
    i.Show(out);
}
