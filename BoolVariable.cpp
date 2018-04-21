#include <iostream>
#include "BoolVariable.h"

using namespace std;

/*
 * constructor of new bool variable.
 * value - the given value of the variable.
 * name - the given name of the variable.
 * type - the type of the variable
 */
BoolVariable::BoolVariable(bool value, string name, string type) : Variable(name,type)
{
    m_value = value;
}


/*
 *destructor of the variable.
 */
BoolVariable::~BoolVariable() {}

/*
 * sets new value of the current variable.
 * value - the new value of the variable.
 */
void BoolVariable::setValue(bool value)
{
    m_value = value;
}

/*
 * return the value of the variable.
 */
bool BoolVariable::getValue() const
{
    return m_value;
}

/*
 * copying a given bool variable into te current variable.
 * b1 - the given variable.
 */
BoolVariable& BoolVariable::operator=(const BoolVariable& b1)
{
    bool a = b1.getValue();
    setValue(a);
    return *this;
}

/*
 * returns true or false is the value of the current variable is identical to the value of
 * the given variable.
 * b1 - the given variable
 */
bool BoolVariable::operator==(const BoolVariable& b1)const
{
    bool a = b1.getValue();
    bool b = getValue();
    return a == b;
}

/*
 * A private function to print the variable.
 */
ostream & BoolVariable::Show(ostream & out)const
{
    if(m_value)
        cout<<"true";
    else
        cout<<"false";
}

/*
 * printing a given variable with the right format
 */
ostream & operator<<(ostream& out,const BoolVariable & b)
{
    b.Show(out);
}
