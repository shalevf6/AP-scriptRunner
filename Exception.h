/*/
 * An abstract class that represents the exceptions that can be thrown while the interperter works
 */
#ifndef ASS_4_ERROR_H
#define ASS_4_ERROR_H

#include <iostream>

using namespace std;

class Exception
{
protected:
    int m_rowNum;
public:
    Exception(int rowNum):m_rowNum(rowNum){}
    virtual void Handle()const=0;
};

/*/
 * A class that represents a Bad Variable Exception
 */
class BadVariableTypeException : public Exception
{
public:
    BadVariableTypeException(int rowNum): Exception(rowNum) {}
    virtual void Handle()const{cout<<"Bad variable type exception in row number: "<<m_rowNum;}
};

/*/
 * A class that represents a Variable Not Found
 */
class VariableNotFoundException : public Exception
{
public:
    VariableNotFoundException(int rowNum): Exception(rowNum) {}
    virtual void Handle()const{cout<<"Variable not found exception in row number: "<<m_rowNum;}
};

/*/
 * A class that represents an Array Out Of Bound Exception
 */
class ArrayOutOfBoundException : public Exception
{
public:
    ArrayOutOfBoundException(int rowNum): Exception(rowNum) {}
    virtual void Handle()const{cout<<"Array out of bounds exception in row number: "<<m_rowNum;}
};


#endif //ASS_4_ERROR_H
