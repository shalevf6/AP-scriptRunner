/*/
 * A class that represents a variable of type int
 */
#ifndef ASS_4_INTVARIABLE_H
#define ASS_4_INTVARIABLE_H
#include <iostream>

using namespace std;
#include "Variable.h"

class IntVariable : public Variable {

private:
    int m_intValue;
    ostream & Show(ostream & out)const;
public:
    IntVariable(int newVal, string name, string type="int");
    ~IntVariable();
    int getVal()const;
    void setVal(int newVal);
    IntVariable& operator+(const IntVariable& addTo);
    IntVariable& operator+=(const IntVariable& addToPlusOne);
    IntVariable& operator=(const IntVariable& newVal);
    bool operator==(const IntVariable& equalTo)const;
    friend ostream & operator<<(ostream& out,const IntVariable& i);
};


#endif //ASS_4_INTVARIABLE_H
