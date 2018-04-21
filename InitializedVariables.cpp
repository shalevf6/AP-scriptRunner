#include <iostream>
#include "InitializedVariables.h"

using namespace std;

InitializedVariables::InitializedVariables(): m_varName("noNameFirst")
{
    m_array = (Variable**) malloc(sizeof(Variable) * 2);
    m_numOfElements = 0;
    m_sizeOfArray = 2;
}

InitializedVariables::~InitializedVariables()
{
    free(m_array);
}

void InitializedVariables::addVariable(Variable& name)
{
    if(m_numOfElements == m_sizeOfArray)
        increaseArraySize();
    m_array[m_numOfElements] = &name;
    m_numOfElements++;
}

void InitializedVariables::replaceVariable(Variable& name)
{
    string type = m_array[0]->getType();
    if(type != "variable")
        delete(m_array[0]);
    m_numOfElements = 0;
    m_varName = name.getName();
    addVariable(name);
}

void InitializedVariables::increaseArraySize()
{
    m_sizeOfArray = m_sizeOfArray * 2;
    m_array = (Variable**) realloc(m_array,sizeof(Variable) * m_sizeOfArray);
}

bool InitializedVariables::doesNameExists(string name)
{
    int i;
    for(i = 0; i < m_numOfElements; i++)
        if(m_array[i]->getName() == name)
            return true;
    return false;
}

Variable& InitializedVariables::getVariable(string name)
{
    int i;
    for(i = 0; i < m_numOfElements; i++)
        if(m_array[i]->getName() == name)
            return *m_array[i];
}

Variable& InitializedVariables::getFirstVariable()
{
    return *m_array[0];
}

string InitializedVariables::getVarName()
{
    return m_varName;
}

void InitializedVariables::deleteAll()
{
    int i;
    for (i = 0; i < m_numOfElements; i++) {
        delete (m_array[i]);
    }
}
