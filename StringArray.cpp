#include <iostream>
#include "StringArray.h"

using namespace std;

/*
 * constructor for new string array using array variable constructor.
 * stringArray - points to the beginning of the new array.
 * size - the size of the array.
 * name- the given name of the array
 * type - the type of the array - set by default to be "stringArray".
 *
 */
StringArray::StringArray(string *stringArray, int size, string name, string type): ArrayVariable(size,name,type)
{
    m_stringArray = (StringVariable**) malloc(sizeof(StringVariable) * size);
    int i;
    for (i = 0; i < size; i++)
    {
        m_stringArray[i] = new StringVariable(stringArray[i],"","string");
    }
}
/*
 * destructor of the array.
 */

StringArray::~StringArray()
{
    int i;
    for (i = 0; i < ArrayVariable::m_arraySize; i++)
        delete(m_stringArray[i]);
    free(m_stringArray);
}

/*
 * returns a pointer of an element in the array.
 * index - the given index of the element.
 */

StringVariable* StringArray::getStringElement(int index)
{
    return m_stringArray[index];
}

/*
 *  A private function to print the array.
 */
ostream & StringArray::Show(ostream & out)const
{
    int i;
    cout << *m_stringArray[0];
    int length = ArrayVariable::m_arraySize;
    for (i = 1; i < length; i++)
    {
        cout << ", ";
        cout << *m_stringArray[i];
    }
    cout << "\n";
}

/*
 * printing a given array with the right format.
 */
ostream & operator<<(ostream& out,const StringArray & sA)
{
    sA.Show(out);
}
