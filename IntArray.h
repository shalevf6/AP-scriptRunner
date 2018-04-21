/*/
 * A class that represents an array of IntVariable objects
 */
#ifndef ASS_4_INTARRAY_H
#define ASS_4_INTARRAY_H

#include <iostream>
#include <cstdlib>
#include "ArrayVariable.h"
#include "IntVariable.h"

using namespace std;

class IntArray : public ArrayVariable {
private:
    IntVariable** m_intArray;
    ostream & Show(ostream & out)const;
public:
    IntArray(int intArray[],int size, string name, string type="intArray");
    ~IntArray();
    IntVariable* getIntElement(int index);
    friend ostream & operator<<(ostream& out,const IntArray& iA);
};


#endif //ASS_4_INTARRAY_H
