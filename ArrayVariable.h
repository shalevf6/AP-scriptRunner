/*/
 * A class that represents n abstract array
 */
#ifndef ASS_4_ARRAYVARIABLE_H
#define ASS_4_ARRAYVARIABLE_H

#include "Variable.h"
#include <iostream>

using namespace std;

class ArrayVariable : public Variable {
protected:
    int m_arraySize;
public:
    ArrayVariable(int arraySize=0, string name="", string type="array");
    virtual ~ArrayVariable();
    int getSize();
};

#endif //ASS_4_ARRAYVARIABLE_H