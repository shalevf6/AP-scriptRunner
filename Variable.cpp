#include <iostream>
#include "Variable.h"

using namespace std;

/*
 * constructor of new variable.
 * name - the given name of the variable.
 * type - the type of the variable
 */
Variable::Variable(string name,string type) : m_name(name),m_type(type)
{

}

/*
 * destructor of variable.
 */
Variable::~Variable() {}

/*
 * return the name of the current variable.
 */
string Variable::getName()const
{
    return m_name;
}

/*
 * return the type of the current variable.
 */
string Variable::getType() const
{
    return m_type;
}
