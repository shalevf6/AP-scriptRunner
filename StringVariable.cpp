#include <iostream>
#include "StringVariable.h"

using namespace std;

/*
 * constructor of new string variable.
 * value - the given value of the variable.
 * name - the given name of the variable.
 * type - the type of the variable
 */
StringVariable::StringVariable(string value, string name, string type):m_value(value) , Variable(name,type) {}

/*
 *destructor of the variable.
 */

StringVariable::~StringVariable() {}

/*
 * sets new value of the current variable.
 * value - the new value of the variable.
 */
void StringVariable::setValue(string value)
{
    m_value = value;
}

/*
 * return the value of the variable.
 */

string StringVariable::getValue() const
{
    return m_value;
}

/*
 * copying a given bool variable into te current variable.
 * b1 - the given variable.
 */

StringVariable& StringVariable::operator=(const StringVariable& b1)
{
    string a = b1.getValue();
    setValue(a);
    return *this;
}

/*
 * returns true or false is the value of the current variable is identical to the value of
 * the given variable.
 * b1 - the given variable
 */
bool StringVariable::operator==(const StringVariable& b1)const
{
    string a = b1.getValue();
    string b = getValue();
    return a == b;
}

/*
 * A private function to print the variable.
 */
ostream & StringVariable::Show(ostream & out)const
{
    cout<<m_value;
}

/*
 * printing a given variable with the right format
 */
ostream & operator<<(ostream& out,const StringVariable & s)
{
    s.Show(out);
}
