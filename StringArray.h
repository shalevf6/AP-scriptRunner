/*/
 * A class that represents an array of StringVariable objects
 */
#ifndef ASS_4_STRINGARRAY_H
#define ASS_4_STRINGARRAY_H

#include <iostream>
#include <cstdlib>
#include "ArrayVariable.h"
#include "StringVariable.h"

using namespace std;

class StringArray : public ArrayVariable {
private:
    StringVariable** m_stringArray;
    ostream & Show(ostream & out)const;
public:
    StringArray(string StringArray[],int size, string name, string type="stringArray");
    ~StringArray();
    StringVariable* getStringElement(int index);
    friend ostream & operator<<(ostream& out,const StringArray& sA);
};


#endif //ASS_4_STRINGARRAY_H
