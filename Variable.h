/*/
 * An abstract class that represents a variable
 */
#ifndef ASS_4_VARIABLE_H
#define ASS_4_VARIABLE_H

#include <iostream>

using namespace std;

class Variable {
protected:
    const string m_name;
    const string m_type;
public:
    Variable(string name,string type="variable");
    virtual ~Variable();
    string getName()const;
    string getType()const;
};


#endif //ASS_4_VARIABLE_H