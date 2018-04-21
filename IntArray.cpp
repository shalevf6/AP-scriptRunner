#include <iostream>
#include "IntArray.h"

using namespace std;

/*
 * constructor of new int array
 * intArray - points of the beginning of the new array.
 * size - the size of the array.
 * name - the given name of the array.
 * type - the type of the array, set to be int.
 */
IntArray::IntArray(int *intArray, int size,string name,string type): ArrayVariable(size,name,type)
{
    m_intArray = (IntVariable**) malloc(sizeof(IntVariable) * size);
    int i;
    for (i = 0; i < size; i++)
    {
        m_intArray[i] = new IntVariable(intArray[i],"","int");
    }
}
/*
 * destructor of the array.
 */
IntArray::~IntArray()
{
    int i;
    for (i = 0; i < ArrayVariable::m_arraySize; i++)
        delete(m_intArray[i]);
    free(m_intArray);
}

/*
 * returns a pointer of an element in the array.
 * index - the given index of the element.
 */
IntVariable* IntArray::getIntElement(int index)
{
    return m_intArray[index];
}

/*
 * A private function to print the array.
 */
ostream & IntArray::Show(ostream & out)const
{
    int i;
    cout << *m_intArray[0];
    int length = ArrayVariable::m_arraySize;
    for (i = 1; i < length; i++)
    {
        cout << ", ";
        cout << *m_intArray[i];
    }
    cout << "\n";
}

/*
 * printing a given array with the right format.
 */
ostream & operator<<(ostream& out,const IntArray & iA)
{
    iA.Show(out);
}
