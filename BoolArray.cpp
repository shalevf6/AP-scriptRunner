#include "BoolArray.h"
#include <iostream>
using namespace std;

/*
 * constructor for new bool array using array variable constructor.
 * boolArray - points to the beginning of the new array.
 * size - the size of the array.
 * name- the given name of the array
 * type - the type of the array - set by default to be "BoolArray".
 *
 */
BoolArray::BoolArray(bool* boolArray,int size, string name, string type) : ArrayVariable(size,name,type)
{
    m_boolArray = (BoolVariable**) malloc(sizeof(BoolVariable) * size);
    for (int i = 0; i < size;i++)
        m_boolArray[i] = new BoolVariable(boolArray[i],"","bool");
}

/*
 * destructor of the array.
 */
BoolArray::~BoolArray()
{
    for (int i = 0; i < ArrayVariable::m_arraySize ; i++)
        delete(m_boolArray[i]);
    free(m_boolArray);
}

/*
 * returns a pointer of an element in the array.
 * index - the given index of the element.
 */
BoolVariable* BoolArray::getBoolElement(int index)
{
    return m_boolArray[index];
}

/*
 *  A private function to print the array.
 */
ostream & BoolArray::Show(ostream & out)const
{
    int i;
    cout << *m_boolArray[0];
    int length = ArrayVariable::m_arraySize;
    for (i = 1; i < length; i++)
    {
        cout << ", ";
        cout << *m_boolArray[i];
    }
    cout << "\n";
}

/*
 * printing a given array with the right format.
 */
ostream & operator<<(ostream& out,const BoolArray & bA)
{
    bA.Show(out);
}
