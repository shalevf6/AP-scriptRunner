#include <iostream>
#include "ArrayVariable.h"

using namespace std;

/*
 * constructor for new array, using variable constructor.
 * arraySize - the given size of the new array.
 * name - the given name of the new array.
 * type - the type of the array can be int string or bool.
 */
ArrayVariable::ArrayVariable(int arraySize, string name, string type) : Variable(name,type)
{
    m_arraySize = arraySize;
}
/*
 * destructor of the array.
 */
ArrayVariable::~ArrayVariable() {}

/*
 * returns the size of the array.
 */
int ArrayVariable::getSize()
{
    return m_arraySize;
}
