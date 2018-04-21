/*/
 * A class that represents a dynamic array of pointers to objects of type Variable
 */
#ifndef ASS_4_INITIALIZEDVARIABLES_H
#define ASS_4_INITIALIZEDVARIABLES_H

#include <iostream>
#include <cstdlib>
#include "Variable.h"

using namespace std;

class InitializedVariables {
private:
    void increaseArraySize();
    Variable** m_array;
    int m_numOfElements;
    int m_sizeOfArray;
    string m_varName;
public:
    InitializedVariables();
    ~InitializedVariables();
    void addVariable(Variable& name);
    void replaceVariable(Variable& name);
    bool doesNameExists(string name);
    Variable& getVariable(string name);
    Variable& getFirstVariable();
    string getVarName();
    void deleteAll();
};


#endif //ASS_4_INITIALIZEDVARIABLES_H
