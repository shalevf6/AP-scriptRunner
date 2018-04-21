/*/
 * A class that represents an array of BoolVariable objects
 */
#ifndef ASS_4_BOOLARRAY_H
#define ASS_4_BOOLARRAY_H
#include <iostream>
#include <cstdlib>
using namespace std;
#include "ArrayVariable.h"
#include "BoolVariable.h"

class BoolArray : public ArrayVariable {
private:
    BoolVariable** m_boolArray;
    ostream & Show(ostream & out)const;
public:
    BoolArray(bool m_boolArr[],int size, string name, string type="boolArray");
    ~BoolArray();
    BoolVariable* getBoolElement(int index);
    friend ostream & operator<<(ostream& out,const BoolArray& bA);
};


#endif //ASS_4_BOOLARRAY_H
