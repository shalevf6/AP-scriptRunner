/*/
 * A class that represents a variable of type bool
 */
#ifndef ASS_4_BOOLVARIABLE_H
#define ASS_4_BOOLVARIABLE_H

#include "Variable.h"
#include <iostream>

using namespace std;

class BoolVariable : public Variable {
private:
    bool m_value;
    ostream & Show(ostream & out)const;
public:
    BoolVariable(bool value, string name, string type="bool");
    ~BoolVariable();

    void setValue(bool value);
    bool getValue() const;
    BoolVariable& operator=(const BoolVariable& b1);
    bool operator==(const BoolVariable& m1)const;
    friend ostream & operator<<(ostream& out,const BoolVariable& s);
};


#endif //ASS_4_BOOLVARIABLE_H