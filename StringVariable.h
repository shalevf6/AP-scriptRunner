/*/
 * A class that represents a variable of type string
 */
#ifndef ASS_4_STRINGVARIABLE_H
#define ASS_4_STRINGVARIABLE_H

#include "Variable.h"
#include <iostream>

using namespace std;

class StringVariable : public Variable {
private:
    string m_value;
    ostream & Show(ostream & out)const;
public:
    StringVariable(string value, string name, string type="string");
    ~StringVariable();

    void setValue(string value);
    string getValue() const;
    StringVariable& operator=(const StringVariable& s1);
    bool operator==(const StringVariable& s1)const;
    friend ostream & operator<<(ostream& out,const StringVariable& s);
};


#endif //ASS_4_STRINGVARIABLE_H