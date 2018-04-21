#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "InitializedVariables.h"
#include "StringArray.h"
#include "IntArray.h"
#include "BoolArray.h"
#include "Exception.h"

using namespace std;

/*
 * return true or false if a given string is a number.
 * s - the given string that checked.
 */
bool isNumber(const string& s) {
    bool ans = (s.find_first_not_of("0123456789") == string::npos);
    return ans;
}

/*
 * put in s a string of a given number
 * x - the given number.
 */
void intToString(int x,string& s)
{
    ostringstream convert;
    convert << x;
    s = convert.str();
}

/*
 * return the number of a given string which contains a number.
 * s - the given string.
 */
int stringToInt(const string& s)
{
    return atoi(s.c_str());
}

/*
 * returns true of false if the two string contains the same type, int and int array
 * considered to be the same type.
 * type1 - the first gven type.
 * type2 - the second given type.
 */
bool areTheyTheSameType (const string& type1, const string& type2)
{
    return ((type1 == "int" && type2 == "int") || (type1 == "intArray" && type2 == "intArray") || (type1 == "int" && type2 == "intArray") || (type1 == "intArray" && type2 == "int") ||
            (type1 == "bool" && type2 == "bool") || (type1 == "boolArray" && type2 == "boolArray") || (type1 == "bool" && type2 == "boolArray") || (type1 == "boolArray" && type2 == "bool") ||
            (type1 == "string" && type2 == "string") || (type1 == "stringArray" && type2 == "stringArray") || (type1 == "string" && type2 == "stringArray") || (type1 == "stringArray" && type2 == "string"));
}

/*
 * return a temp variable for future check that the given line is legal.
 * first the given line is checked to be legal, meaning that no setting
 * different value is certain variable or an certain array.
 * also checking correct using of the operators and multi operators using
 * in the given line.
 * if the line is illegal an error will be thrown.
 * line - the given line.
 * lineNum - the number of the line.
 * columnNum - index of the beginning of new name , command or operator.
 * initializedVariables - the current initialized variables.
 */
Variable& normalParse(string line,int lineNum,int columnNum,InitializedVariables &initializedVariables)
{
    InitializedVariables tempArr = InitializedVariables();
    IntVariable temp = IntVariable(0,"","variable");
    tempArr.addVariable(temp);
    while( columnNum < line.length()) {
        if (tempArr.getVarName() == "noNameFirst" && line[columnNum] != '#') {
            int nameLength = 0;
            int demiI = columnNum;
            bool isArrCellOp1 = false;
            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                    line[demiI] != '}')) {
                if(line[demiI] == '[')
                    isArrCellOp1 = true;
                if(line[demiI] == ']')
                    isArrCellOp1 = false;
                nameLength++;
                demiI++;
            }
            string name1 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
            bool isArr1 = false;
            string arrName1;
            string arrLoc1;
            if (name1.find('[') != std::string::npos) {
                if(name1[0] == '[') {
                    isArr1 = true;
                }
                else {
                    int iter1 = 0;
                    while (name1[iter1] != '[')
                        iter1++;
                    arrName1 = name1.substr(0, (unsigned long) iter1);
                    iter1++;
                    int iter2 = 0;
                    int counter = iter1;
                    while (name1[counter] != ']') {
                        iter2++;
                        counter++;
                    }
                    arrLoc1 = name1.substr((unsigned long) iter1, (unsigned long) iter2);
                    name1 = arrName1;
                    isArr1 = true;
                    if (!(isNumber(arrLoc1))) {
                        if (initializedVariables.doesNameExists(arrLoc1)) {
                            if (initializedVariables.getVariable(arrLoc1).getType() == "int") {
                                int value = ((IntVariable &) initializedVariables.getVariable(arrLoc1)).getVal();
                                intToString(value, arrLoc1);
                            } else {
                                throw BadVariableTypeException(lineNum);
                            }
                        } else {
                            if(arrLoc1.find('+') != std::string::npos || arrLoc1.find('=') != std::string::npos) {
                                Variable &ans = normalParse(arrLoc1, lineNum, 0, initializedVariables);
                                if(ans.getType() != "int")
                                    throw BadVariableTypeException(lineNum);
                                int value = ((IntVariable &)ans).getVal();
                                delete(&ans);
                                intToString(value, arrLoc1);
                            } else
                                throw VariableNotFoundException(lineNum);
                        }
                    }
                }
            }
            columnNum = demiI;
            if (line[columnNum] == '+') {                                         // INT+
                if (initializedVariables.doesNameExists(name1)) {               // NAME1+
                    // CHECK IF NAME1 IS TYPE INT, IF NOT THROW BAD VARIABLE TYPE EXCEPTION
                    if (initializedVariables.getVariable(name1).getType() != "int" &&
                        initializedVariables.getVariable(name1).getType() != "intArray")
                        throw BadVariableTypeException(lineNum);
                    if (isArr1) {
                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                        int arrSize1 = iA1.getSize();
                        int arrLocInt1 = stringToInt(arrLoc1);
                        if (arrLocInt1 > arrSize1 - 1)
                            throw ArrayOutOfBoundException(lineNum);
                    }
                    if (line[columnNum + 1] == '=') {                                // NAME1+=
                        columnNum = columnNum + 2;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {                  // NAME1+=NAME2
                            // CHECK IF ITS AN INT VARIABLE
                            if (initializedVariables.getVariable(name2).getType() != "int" &&
                                initializedVariables.getVariable(name2).getType() != "intArray")
                                throw BadVariableTypeException(lineNum);
                            if (isArr2) {
                                IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                int arrSize2 = iA2.getSize();
                                int arrLocInt2 = stringToInt(arrLoc2);
                                if (arrLocInt2 > arrSize2 - 1)
                                    throw ArrayOutOfBoundException(lineNum);
                                if (isArr1) {                           // ARR[NAM1]+=ARR[NAME2]
                                    IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                    int arrLocInt1 = stringToInt(arrLoc1);
                                    IntVariable &i1 = (IntVariable &) *(iA1.getIntElement(arrLocInt1));
                                    IntVariable &i2 = (IntVariable &) *(iA2.getIntElement(arrLocInt2));
                                    i1 += i2;
                                    IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]",
                                                                           "int");
                                    *intTemp = i1;
                                    tempArr.replaceVariable(*intTemp);
                                } else {                              // NAME1+=ARR[NAME2]
                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                    IntVariable &i2 = (IntVariable &) *(iA2.getIntElement(arrLocInt2));
                                    i1 += i2;
                                    IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                    *intTemp = i1;
                                    tempArr.replaceVariable(*intTemp);
                                }
                            } else {
                                if (isArr1) {                        // ARR[NAME1]+=NAME2
                                    IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                    int arrLocInt1 = stringToInt(arrLoc1);
                                    IntVariable &i1 = (IntVariable &) *(iA1.getIntElement(arrLocInt1));
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    i1 += i2;
                                    IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]",
                                                                           "int");
                                    *intTemp = i1;
                                    tempArr.replaceVariable(*intTemp);
                                } else {                                 // NAME1+=NAME2
                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    i1 += i2;
                                    IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                    *intTemp = i1;
                                    tempArr.replaceVariable(*intTemp);
                                    // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                }
                            }
                        } else {                                           // NAME1+=INT
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            if (name2 == "true" || name2 == "false") {
                                throw BadVariableTypeException(lineNum);
                            } else {
                                if (name2[0] == '"') {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '[') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        if (isNumber(name2)) {
                                            int value = stringToInt(name2);
                                            IntVariable i2 = IntVariable(value, "", "int");
                                            if (isArr1) {                         // ARR[NAME1]+=INT
                                                IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                                int arrLocInt1 = stringToInt(arrLoc1);
                                                IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                                i1 += i2;
                                                IntVariable *intTemp = new IntVariable(0,
                                                                                       iA1.getName() + "[" + arrLoc1 +
                                                                                       "]",
                                                                                       "int");
                                                *intTemp = i1;
                                                tempArr.replaceVariable(*intTemp);
                                            } else {                               // NAME1+=INT
                                                IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(
                                                        name1);
                                                i1 += i2;
                                                IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                                *intTemp = i1;
                                                tempArr.replaceVariable(*intTemp);
                                            }
                                        } else {
                                            // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                            throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                        }
                    } else {                         // NAME1+
                        columnNum++;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {            // NAME1+NAME2
                            // CHECK IF ITS AN INT VARIABLE
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            if (initializedVariables.getVariable(name2).getType() != "int" &&
                                initializedVariables.getVariable(name2).getType() != "intArray")
                                throw BadVariableTypeException(lineNum);
                            if (isArr2) {
                                IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                int arrSize2 = iA2.getSize();
                                int arrLocInt2 = stringToInt(arrLoc2);
                                if (arrLocInt2 > arrSize2 - 1)
                                    throw ArrayOutOfBoundException(lineNum);
                                if (isArr1) {                              // ARR[NAME1]+ARR[NAME2]
                                    IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                    int arrLocInt1 = stringToInt(arrLoc1);
                                    IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    IntVariable &intTemp = i1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                } else {                                 // NAME1+ARR[NAME2]
                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    IntVariable &intTemp = i1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                }
                            } else {
                                if (isArr1) {                            // ARR[NAME1]+NAME2
                                    IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                    int arrLocInt1 = stringToInt(arrLoc1);
                                    IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    IntVariable &intTemp = i1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                } else {                                 // NAME1+NAME2
                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    // IF ITS AN INT, COMBINE THEM AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    IntVariable &intTemp = i1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                }
                            }
                        } else {                             // NAME1+INT
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            if (name2[0] == '"') {
                                throw BadVariableTypeException(lineNum);
                            } else {
                                if (name2[0] == '[') {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    // IF ITS AN INT VARIABLE COMBINE THEM AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    if (isNumber(name2)) {
                                        int value = stringToInt(name2);
                                        IntVariable i2 = IntVariable(value, "", "int");
                                        if (isArr1) {                              // ARR[NAME]+INT
                                            IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                            int arrLocInt1 = stringToInt(arrLoc1);
                                            IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                            IntVariable &intTemp = i1 + i2;
                                            tempArr.replaceVariable(intTemp);
                                        } else {                                    // NAME+INT
                                            IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                            IntVariable &intTemp = i1 + i2;
                                            tempArr.replaceVariable(intTemp);
                                        }
                                    } else {
                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                        throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                    }
                } else {                                 // INT+
                    // CHECK IF IT'S AN INT VARIABLE
                    if (!isNumber(name1))
                        throw BadVariableTypeException(lineNum);
                    if (line[columnNum + 1] == '=') {              // INT+=
                        columnNum = columnNum + 2;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {          // INT+=NAME2
                            // CHECK IF ITS AN INT VARIABLE
                            if (initializedVariables.getVariable(name2).getType() != "int" &&
                                initializedVariables.getVariable(name2).getType() != "intArray")
                                throw BadVariableTypeException(lineNum);
                            if (isArr2) {                             // INT+=ARR[NAME2]
                                IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                int arrSize2 = iA2.getSize();
                                int arrLocInt2 = stringToInt(arrLoc2);
                                if (arrLocInt2 > arrSize2 - 1)
                                    throw ArrayOutOfBoundException(lineNum);
                                IntVariable i1 = IntVariable(stringToInt(name1), "", "int");
                                IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                i1 += i2;
                                IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                *intTemp = i1;
                                tempArr.replaceVariable(*intTemp);
                            } else {                                 // INT+=NAME2
                                IntVariable i1 = IntVariable(stringToInt(name1), "", "int");
                                IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                i1 += i2;
                                IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                *intTemp = i1;
                                tempArr.replaceVariable(*intTemp);
                            }
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                        } else {                                         // INT+=INT
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            if (name2 == "true" || name2 == "false") {
                                throw BadVariableTypeException(lineNum);
                            } else {
                                if (name2[0] == '"') {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '[') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        if (isNumber(name2)) {
                                            int value = stringToInt(name2);
                                            IntVariable i2 = IntVariable(value, "", "int");
                                            IntVariable i1 = IntVariable(stringToInt(name1), "", "int");
                                            i1 += i2;
                                            IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                            *intTemp = i1;
                                            tempArr.replaceVariable(*intTemp);
                                        } else {
                                            // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                            throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                        }
                    } else {                // INT+
                        columnNum++;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {              // INT+NAME2
                            // CHECK IF ITS AN INT VARIABLE
                            if (initializedVariables.getVariable(name2).getType() != "int" &&
                                initializedVariables.getVariable(name2).getType() != "intArray")
                                throw BadVariableTypeException(lineNum);
                            if (isArr2) {                             // INT+ARR[NAME2]
                                IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                int arrSize2 = iA2.getSize();
                                int arrLocInt2 = stringToInt(arrLoc2);
                                if (arrLocInt2 > arrSize2 - 1)
                                    throw ArrayOutOfBoundException(lineNum);
                                IntVariable i1 = IntVariable(stringToInt(name1), "", "int");
                                IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                IntVariable &intTemp = i1 + i2;
                                tempArr.replaceVariable(intTemp);
                            } else {                                 // INT+NAME2
                                IntVariable i1 = IntVariable(stringToInt(name1), "", "int");
                                IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                IntVariable &intTemp = i1 + i2;
                                tempArr.replaceVariable(intTemp);
                            }
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                        } else {                                       // INT+INT
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                            // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            if (name2 == "true" || name2 == "false") {
                                throw BadVariableTypeException(lineNum);
                            } else {
                                if (name2[0] == '"') {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '[') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        if (isNumber(name2)) {
                                            int value = stringToInt(name2);
                                            IntVariable i2 = IntVariable(value, "", "int");
                                            IntVariable i1 = IntVariable(stringToInt(name1), "", "int");
                                            IntVariable &intTemp = i1 + i2;
                                            tempArr.replaceVariable(intTemp);
                                        } else {
                                            // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                            throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (line[columnNum] == '=') {                         // VAR= // NAME1=
                if (initializedVariables.doesNameExists(name1)) {
                    if (isArr1) {
                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                        int arrSize1 = iA1.getSize();
                        int arrLocInt1 = stringToInt(arrLoc1);
                        if (arrLocInt1 > arrSize1 - 1)
                            throw ArrayOutOfBoundException(lineNum);
                    }
                    if (line[columnNum + 1] == '=') {                     // NAME1==
                        // CHECK ITS TYPE AND SAVE IT
                        columnNum = columnNum + 2;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {           // NAME1==NAME2
                            // CHECK IF ITS THE SAME TYPE AS NAME1
                            // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF NOT, THROW BAD VARIABLE TYPE EXCEPTION
                            string typeName1 = initializedVariables.getVariable(name1).getType();
                            string typeName2 = initializedVariables.getVariable(name2).getType();
                            if (!(areTheyTheSameType(typeName1,typeName2)))
                                throw BadVariableTypeException(lineNum);
                            if (typeName1 == typeName2) //same
                            {
                                if (typeName1 == "int" || typeName1 == "intArray")  //int or intarray
                                {
                                    if (typeName1 == "int")             //int==int
                                    {
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                    else            //int array == int array
                                    {
                                        IntArray &Ai1 = (IntArray &) initializedVariables.getVariable(name1);
                                        IntArray &Ai2 = (IntArray &) initializedVariables.getVariable(name2);

                                        IntVariable *locarr1 = Ai1.getIntElement((stringToInt(arrLoc1)));
                                        IntVariable *locarr2 = Ai2.getIntElement((stringToInt(arrLoc2)));

                                        IntVariable &i1 = (IntVariable &) (*Ai1.getIntElement(locarr1->getVal()));
                                        IntVariable &i2 = (IntVariable &) (*Ai2.getIntElement(locarr2->getVal()));
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }

                                } else
                                if (typeName1 == "string" || typeName1 == "stringArray")  //string or string array
                                {
                                    if (typeName1 == "string")  //string==string
                                    {
                                        StringVariable &i1 = (StringVariable &) initializedVariables.getVariable(name1);
                                        StringVariable &i2 = (StringVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    } else        //string array == string array
                                    {
                                        StringArray &Ai1 = (StringArray &) initializedVariables.getVariable(name1);
                                        StringArray &Ai2 = (StringArray &) initializedVariables.getVariable(name2);

                                        StringVariable &i1 = (StringVariable &) (*Ai1.getStringElement((stringToInt(arrLoc1))));
                                        StringVariable &i2 = (StringVariable &) (*Ai2.getStringElement((stringToInt(arrLoc2))));
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                } else
                                if (typeName1 == "bool" || typeName1 == "boolArray")  //bool or boolarray
                                {
                                    if (typeName1 == "bool") {  //bool==bool
                                        BoolVariable &i1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                        BoolVariable &i2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    } else        //boolarray == boolarray
                                    {
                                        BoolArray &Ai1 = (BoolArray &) initializedVariables.getVariable(name1);
                                        BoolArray &Ai2 = (BoolArray &) initializedVariables.getVariable(name2);

                                        BoolVariable &i1 = (BoolVariable &) (*Ai1.getBoolElement((stringToInt(arrLoc1))));
                                        BoolVariable &i2 = (BoolVariable &) (*Ai2.getBoolElement((stringToInt(arrLoc2))));
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                }
                            } else {            //not the same
                                if (typeName1 == "int" || typeName1 == "intArray")  //int or intarray
                                {
                                    if (typeName1 == "int")             //int == intarray[loc]
                                    {
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntArray &Ai2 = (IntArray &) initializedVariables.getVariable(name2);
                                        IntVariable &i2 = (IntVariable &) (*Ai2.getIntElement((stringToInt(arrLoc2))));
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    } else            //int array[loc]  == int
                                    {
                                        IntArray &Ai1 = (IntArray &) initializedVariables.getVariable(name1);
                                        IntVariable &i1 = (IntVariable &) (*Ai1.getIntElement((stringToInt(arrLoc1))));
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                } else if (typeName1 == "string" || typeName1 == "stringArray")  //string or stringarray
                                {
                                    if (typeName1 == "string")  //string = stringarray[loc]
                                    {
                                        StringVariable &i1 = (StringVariable &) initializedVariables.getVariable(name1);
                                        StringArray &Ai2 = (StringArray &) initializedVariables.getVariable(name2);
                                        StringVariable &i2 = (StringVariable &) (*Ai2.getStringElement((stringToInt(arrLoc2))));
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    } else        //string array[loc] == string
                                    {
                                        StringArray &As1 = (StringArray &) initializedVariables.getVariable(name1);
                                        StringVariable &s1 = (StringVariable &) (*As1.getStringElement((stringToInt(arrLoc1))));
                                        StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(s1 == s2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                } else if (typeName1 == "bool" || typeName1 == "boolArray")  //bool or boolarray
                                {
                                    if (typeName1 == "bool") {     //bool = boolarray[loc]
                                        BoolArray &Ai1 = (BoolArray &) initializedVariables.getVariable(name1);
                                        BoolVariable &i1 = (BoolVariable &) (*Ai1.getBoolElement((stringToInt(arrLoc1))));
                                        BoolVariable &i2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    } else {        //boolarray[loc] == bool
                                        BoolArray &Ai1 = (BoolArray &) initializedVariables.getVariable(name1);
                                        BoolVariable &i2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable &i1 = (BoolVariable &) (*Ai1.getBoolElement((stringToInt(arrLoc1))));
                                        BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                }
                            }
                        } else {                                          // NAME1==VAR
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer)
                            // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                            // CHECK IF ITS THE SAME TYPE AS NAME1
                            // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF NOT, THROW BAD VARIABLE TYPE EXCEPTION
                            string typeName1 = initializedVariables.getVariable(name1).getType();
                            if (name2 == "true" || name2 == "false") {//var is bool
                                bool value = (name2 == "true");
                                if (isArr1) {        //arr[loc] == bool
                                    BoolArray &Ai1 = (BoolArray &) initializedVariables.getVariable(name1);
                                    BoolVariable &i1 = (BoolVariable &) (*Ai1.getBoolElement((stringToInt(arrLoc1))));
                                    BoolVariable i2 = BoolVariable(value,"","bool");
                                    BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                } else if (typeName1 == "bool") {   //bool ==bool
                                    BoolVariable &i1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                    BoolVariable i2 = BoolVariable(value,"","bool");
                                    BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                } else
                                    throw BadVariableTypeException(lineNum);
                            } else if (isNumber((name2))) {         //var is num
                                int value = stringToInt(name2);
                                if (isArr1) {        //arr[loc] == bool
                                    IntArray &Ai1 = (IntArray &) initializedVariables.getVariable(name1);
                                    IntVariable &i1 = (IntVariable &) (*Ai1.getIntElement((stringToInt(arrLoc1))));
                                    IntVariable i2 = IntVariable(value,"","int");
                                    BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                } else if (typeName1 == "int") {   //int == int
                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                    IntVariable i2 = IntVariable(value,"","int");
                                    BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                } else
                                    throw BadVariableTypeException(lineNum);
                            } else if (name2[0] == '"') {           //var is string
                                string value = name2.substr(1,name2.length() - 2);
                                if (isArr1) {        //arr[loc] == bool
                                    StringArray &Ai1 = (StringArray &) initializedVariables.getVariable(name1);
                                    StringVariable &i1 = (StringVariable &) (*Ai1.getStringElement((stringToInt(arrLoc1))));
                                    StringVariable i2 = StringVariable(value,"","string");
                                    BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                } else if (typeName1 == "string") {   //string == string
                                    StringVariable &i1 = (StringVariable &) initializedVariables.getVariable(name1);
                                    StringVariable i2 = StringVariable(value,"","string");
                                    BoolVariable *boolTemp = new BoolVariable(i1 == i2, "noName", "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                } else
                                    throw VariableNotFoundException(lineNum);
                            }
                        }
                    } else {                            // NAME1=
                        // CHECK ITS TYPE AND SAVE IT
                        columnNum++;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {          // NAME1=NAME2
                            // CHECK IF ITS THE SAME TYPE AS NAME1
                            string typeName1 = initializedVariables.getVariable(name1).getType();
                            string typeName2 = initializedVariables.getVariable(name2).getType();
                            // IF YES, PUT ITS VALUE INTO THE NEW VARIABLE AND SAVE THE VALUE OF THE NEW VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF NOT, THROW BAD VARIABLE TYPE EXCEPTION
                            if (!(areTheyTheSameType(typeName1,typeName2)))
                                throw BadVariableTypeException(lineNum);
                            if (typeName1 == "int" || typeName1 == "intArray")     //both are int or int array or mix
                            {
                                if (isArr2){
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    if (isArr1) {                           // Name1[arrLoc1]=Name2[arrLoc2]
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]","int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);

                                    } else {                              // Name1=Name2[arrLoc2]
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                } else {
                                    if (isArr1) {                        // Name1[arrLoc1]=Name2
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]", "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    } else {                                 // NAME1=NAME2
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                        // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                    }
                                }

                            } else if (typeName1 == "bool" || typeName1 == "boolArray"){           // both are bool or arr bool or mix
                                if (isArr2){
                                    BoolArray &bA2 = (BoolArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = bA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    if (isArr1) {                           // Name1[arrLoc1]=Name2[arrLoc2]
                                        BoolArray &bA1 = (BoolArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        BoolVariable &b1 = (BoolVariable &) (*bA1.getBoolElement(arrLocInt1));
                                        BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                        b1 = b2;
                                        BoolVariable *boolTemp = new BoolVariable(false, bA1.getName() + "[" + arrLoc1 + "]","bool");
                                        *boolTemp = b1;
                                        tempArr.replaceVariable(*boolTemp);

                                    } else {                              // Name1=Name2[arrLoc2]
                                        BoolVariable &b1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                        BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                        b1 = b2;
                                        BoolVariable *boolTemp = new BoolVariable(false, b1.getName(), "bool");
                                        *boolTemp = b1;
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                } else {
                                    if (isArr1) {                        // Name1[arrLoc1]=Name2
                                        BoolArray &bA1 = (BoolArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        BoolVariable &b1 = (BoolVariable &) (*bA1.getBoolElement(arrLocInt1));
                                        BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        b1 = b2;
                                        BoolVariable *boolTemp = new BoolVariable(false, bA1.getName() + "[" + arrLoc1 + "]", "bool");
                                        *boolTemp = b1;
                                        tempArr.replaceVariable(*boolTemp);
                                    } else {                                 // NAME1=NAME2
                                        BoolVariable &b1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                        BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        b1 = b2;
                                        BoolVariable *boolTemp = new BoolVariable(false, b1.getName(), "bool");
                                        *boolTemp = b1;
                                        tempArr.replaceVariable(*boolTemp);
                                        // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                    }
                                }

                            }else{              //both are string or arr string or mix
                                if (isArr2){
                                    StringArray &sA2 = (StringArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = sA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    if (isArr1) {                           // Name1[arrLoc1]=Name2[arrLoc2]
                                        StringArray &sA1 = (StringArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        StringVariable &s1 = (StringVariable &) (*sA1.getStringElement(arrLocInt1));
                                        StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                        s1 = s2;
                                        StringVariable *stringTemp = new StringVariable("", sA1.getName() + "[" + arrLoc1 + "]","string");
                                        *stringTemp = s1;
                                        tempArr.replaceVariable(*stringTemp);

                                    } else {                              // Name1=Name2[arrLoc2]
                                        StringVariable &s1 = (StringVariable &) initializedVariables.getVariable(name1);
                                        StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                        s1 = s2;
                                        StringVariable *stringTemp = new StringVariable("", s1.getName(), "string");
                                        *stringTemp = s1;
                                        tempArr.replaceVariable(*stringTemp);
                                    }
                                } else {
                                    if (isArr1) {                        // Name1[arrLoc1]=Name2
                                        StringArray &sA1 = (StringArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        StringVariable &s1 = (StringVariable &) (*sA1.getStringElement(arrLocInt1));
                                        StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                        s1 = s2;
                                        StringVariable *stringTemp = new StringVariable("", sA1.getName() + "[" + arrLoc1 + "]", "string");
                                        *stringTemp = s1;
                                        tempArr.replaceVariable(*stringTemp);
                                    } else {                                 // NAME1=NAME2
                                        StringVariable &s1 = (StringVariable &) initializedVariables.getVariable(name1);
                                        StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        s1 = s2;
                                        StringVariable *stringTemp = new StringVariable("", s1.getName(), "string");
                                        *stringTemp = s1;
                                        tempArr.replaceVariable(*stringTemp);
                                        // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                    }
                                }
                            }
                        } else {                                        // NAME1=VAR
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer)
                            // CHECK IF ITS THE SAME TYPE AS NAME1
                            // IF YES, PUT ITS VALUE INTO THE NEW VARIABLE AND SAVE THE VALUE OF THE NEW VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF NOT, THROW BAD VARIABLE TYPE EXCEPTION
                            string typeName1 = initializedVariables.getVariable(name1).getType();
                            if (name2 == "true" || name2 == "false"){            //name2 is bool
                                if (!(typeName1 == "bool" || typeName1 == "boolArray"))
                                    throw BadVariableTypeException(lineNum);
                                if (isArr1){            //name1[] = var
                                    BoolArray &bA1 = (BoolArray &) initializedVariables.getVariable(name1);
                                    int arrLocInt1 = stringToInt(arrLoc1);
                                    BoolVariable &b1 = (BoolVariable &) (*bA1.getBoolElement(arrLocInt1));
                                    BoolVariable b2 = BoolVariable(name2 == "true","","bool");
                                    b1 = b2;
                                    BoolVariable *boolTemp = new BoolVariable(false, bA1.getName() + "[" + arrLoc1 + "]", "bool");
                                    *boolTemp = b1;
                                    tempArr.replaceVariable(*boolTemp);
                                } else{                //name1 = var
                                    BoolVariable &b1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                    BoolVariable b2 = BoolVariable(name2 == "true","","bool");
                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    b1 = b2;
                                    BoolVariable *boolTemp = new BoolVariable(false, b1.getName(), "bool");
                                    *boolTemp = b1;
                                    tempArr.replaceVariable(*boolTemp);
                                }
                            }else {
                                if (name2[0] == '"') {            //name2 is string
                                    if (!(typeName1 == "string" || typeName1 == "stringArray"))
                                        throw BadVariableTypeException(lineNum);
                                    if (isArr1) {            //name1[] = var
                                        StringArray &sA1 = (StringArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        StringVariable &s1 = (StringVariable &) (*sA1.getStringElement(arrLocInt1));
                                        string value = name2.substr(1,name2.length() -2);
                                        StringVariable s2 = StringVariable(value,"","string");
                                        s1 = s2;
                                        StringVariable *stringTemp = new StringVariable(value, sA1.getName() + "[" + arrLoc1 +
                                                                                               "]", "string");
                                        tempArr.replaceVariable(*stringTemp);
                                    } else {                //name1 = var
                                        string value = name2.substr(1,name2.length() - 2);
                                        StringVariable &s1 = (StringVariable &) initializedVariables.getVariable(name1);
                                        StringVariable s2 = StringVariable(value,"","string");
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        s1 = s2;
                                        StringVariable *stringTemp = new StringVariable("", s1.getName(), "string");
                                        *stringTemp = s1;
                                        tempArr.replaceVariable(*stringTemp);
                                    }
                                } else if (isNumber(name2)) {       //name2 is int
                                    if (!(typeName1 == "int" || typeName1 == "intArray"))
                                        throw BadVariableTypeException(lineNum);
                                    if (isArr1) {            //name1[] = var
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        int tmpIntVal = stringToInt(name2);
                                        IntVariable i2 = IntVariable(tmpIntVal,"","int");
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]", "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    } else {                //name1 = var
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        int tmpIntVal = stringToInt(name2);
                                        IntVariable i2 = IntVariable(tmpIntVal,"","int");
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(tmpIntVal, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                } else
                                    throw BadVariableTypeException(lineNum);
                            }
                        }
                    }
                } else {                                   // VAR=
                    if (line[columnNum + 1] == '=') {               // VAR==
                        // CHECK ITS TYPE AND SAVE IT
                        columnNum = columnNum + 2;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {            // VAR==NAME2
                            // CHECK IF ITS THE SAME TYPE AS NAME1
                            string tmpTypName2 = initializedVariables.getVariable(name2).getType();
                            if (!isArr2) {
                                if (tmpTypName2 == "string") {       //name2 is string
                                    if (name1[0] != '"')
                                        throw BadVariableTypeException(lineNum);
                                    string value = name1.substr(1,name1.length() - 2);
                                    StringVariable s1 = StringVariable(value,"","string");
                                    StringVariable &s2 = (StringVariable&) initializedVariables.getVariable(name2);
                                    BoolVariable *tmpBoolV = new BoolVariable(s1 == s2,"noName","bool");
                                    tempArr.replaceVariable(*tmpBoolV);
                                } else if (tmpTypName2 == "int") {       //name2 is int
                                    if (!isNumber(name1))
                                        throw BadVariableTypeException(lineNum);
                                    int value = stringToInt(name1);
                                    IntVariable i1 = IntVariable(value,"","int");
                                    IntVariable &i2 = (IntVariable&) initializedVariables.getVariable(name2);
                                    BoolVariable *tmpBoolV = new BoolVariable(name1 == name2,"noName","bool");
                                    tempArr.replaceVariable(*tmpBoolV);
                                } else if (tmpTypName2 == "bool") {
                                    if (!(name1 == "true" || name1 == "false"))
                                        throw BadVariableTypeException(lineNum);
                                    bool value = name1 == "true";
                                    BoolVariable b1 = BoolVariable(value,"","bool");
                                    BoolVariable &b2 = (BoolVariable&) initializedVariables.getVariable(name2);
                                    BoolVariable *tmpBoolV = new BoolVariable(name1 == name2,"noName","bool");
                                    tempArr.replaceVariable(*tmpBoolV);
                                }
                            } else          //its array
                            if (tmpTypName2 == "intArray"){
                                if (!isNumber(name1))
                                    throw BadVariableTypeException(lineNum);
                                IntArray &tmpArr = (IntArray&) initializedVariables.getVariable(name2);
                                IntVariable &tmpIntV2 = (IntVariable&) *(tmpArr.getIntElement(stringToInt(arrLoc2)));
                                int tmpIntName1 = stringToInt(name1);
                                IntVariable tmpIntV1 = IntVariable(tmpIntName1,"","int");
                                BoolVariable *tmpBoolV = new BoolVariable(tmpIntV1 == tmpIntV2,"noName","bool");
                                tempArr.replaceVariable(*tmpBoolV);
                            } else
                            if (tmpTypName2 == "boolArray"){
                                if (!(name1 == "true" || name1 == "false"))
                                    throw BadVariableTypeException(lineNum);
                                BoolArray &tmpArr = (BoolArray&) initializedVariables.getVariable(name2);
                                BoolVariable &tmpIntV2 = (BoolVariable&) *(tmpArr.getBoolElement(stringToInt(arrLoc2)));
                                bool tmpBoolName1 = name1 == "true";
                                BoolVariable tmpIntV1 = BoolVariable(tmpBoolName1,"","bool");
                                BoolVariable *tmpBoolV = new BoolVariable(tmpIntV1 == tmpIntV2,"noName","bool");
                                tempArr.replaceVariable(*tmpBoolV);
                            } else
                            if (tmpTypName2 == "stringArray"){
                                if (name1[0] != '"')
                                    throw BadVariableTypeException(lineNum);
                                StringArray &tmpArr = (StringArray&) initializedVariables.getVariable(name2);
                                StringVariable &tmpIntV2 = (StringVariable&) *(tmpArr.getStringElement(stringToInt(arrLoc2)));
                                string value = name1.substr(1,name1.length() - 2);
                                StringVariable tmpIntV1 = StringVariable(value,"","string");
                                BoolVariable *tmpBoolV = new BoolVariable(tmpIntV1 == tmpIntV2,"noName","bool");
                                tempArr.replaceVariable(*tmpBoolV);
                            }
                            // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF NOT, THROW BAD VARIABLE TYPE EXCEPTION
                        } else {                                    // VAR==VAR
                            // CHECK ITS TYPE - true / false / " " / 2 (an integer)
                            // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                            // CHECK IF ITS THE SAME TYPE AS NAME1
                            // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF NOT, THROW BAD VARIABLE TYPE EXCEPTION
                            if (isNumber(name1) && isNumber(name2)){      //both int
                                int name1Val = stringToInt(name1);
                                int name2Val = stringToInt(name2);
                                IntVariable i1 = IntVariable(name1Val,name1,"int");
                                IntVariable i2 =IntVariable(name2Val,name2,"int");
                                BoolVariable *tmpBool = new BoolVariable(i1 == i2,"noName","bool");
                                tempArr.replaceVariable(*tmpBool);
                            }else
                            if ((name1 == "true" || name1 == "false") && (name2 == "true" || name2 == "false")){        //both bool
                                BoolVariable *tmpBool = new BoolVariable((name1 == "true" && (name1 == name2)),"noName","bool");
                                tempArr.replaceVariable(*tmpBool);
                            } else
                            if (name1[0] == '"' && name2[0] == '"'){            //both string
                                string value1 = name1.substr(1, name1.length() -2);
                                string value2 = name2.substr(1, name2.length() -2);
                                StringVariable i1 = StringVariable(value1,name1,"string");
                                StringVariable i2 = StringVariable(value2,name2,"string");
                                BoolVariable *tmpBool = new BoolVariable(i1 == i2,"noName","bool");
                                tempArr.replaceVariable(*tmpBool);
                            } else
                                throw BadVariableTypeException(lineNum);
                        }
                    } else {                       //VAR=
                        columnNum++;
                        demiI = columnNum;
                        nameLength = 0;
                        isArrCellOp1 = false;
                        while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                line[demiI] != '}')) {
                            if(line[demiI] == '[')
                                isArrCellOp1 = true;
                            if(line[demiI] == ']')
                                isArrCellOp1 = false;
                            nameLength++;
                            demiI++;
                        }
                        string name2 = line.substr(columnNum, nameLength);
                        bool isArr2 = false;
                        string arrName2;
                        string arrLoc2;
                        if (name2.find('[') != std::string::npos) {
                            if(name2[0] == '[') {
                                isArr2 = true;
                            }
                            else {
                                int iter3 = 0;
                                while (name2[iter3] != '[')
                                    iter3++;
                                arrName2 = name2.substr(0, (unsigned long)iter3);
                                iter3++;
                                int iter4 = 0;
                                int counter = iter3;
                                while (name2[counter] != ']') {
                                    iter4++;
                                    counter++;
                                }
                                arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                name2 = arrName2;
                                isArr2 = true;
                                if(!(isNumber(arrLoc2))) {
                                    if(initializedVariables.doesNameExists(arrLoc2)) {
                                        if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                            intToString(value,arrLoc2);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc2);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        columnNum = demiI;
                        if (initializedVariables.doesNameExists(name2)) {               // VAR=NAME2
                            // CHECK ITS TYPE AND PUT ITS VALUE INTO THE NEW VARIABLE
                            // SAVE THE VALUE OF THE NEW VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            string tmpTypName2 = initializedVariables.getVariable(name2).getType();
                            if (tmpTypName2 == "int" || tmpTypName2 == "intArray")      //int or int array
                            {
                                if (tmpTypName2 == "int")        //int type
                                {
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    int tmpInt2 = i2.getVal();
                                    IntVariable *toBeAdded = new IntVariable (tmpInt2 , name1 , "int");
                                    initializedVariables.addVariable(*toBeAdded);
                                    IntVariable *intTemp = new IntVariable (tmpInt2 , name1 , "int");
                                    tempArr.replaceVariable(*intTemp);
                                }
                                else            //int array
                                {
                                    IntArray &Ai2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrLocInt = stringToInt(arrLoc2);
                                    IntVariable &i2 = (IntVariable &) (*(Ai2.getIntElement(arrLocInt)));
                                    IntVariable *toBeAdded = new IntVariable(0, name1, "int");
                                    *toBeAdded = i2;
                                    initializedVariables.addVariable(*toBeAdded);
                                    IntVariable *intTemp = new IntVariable(0, name1, "int");
                                    *intTemp = i2;
                                    tempArr.replaceVariable(*intTemp);
                                }
                            }
                            if (tmpTypName2 == "bool" || tmpTypName2 == "boolArray")
                            {
                                if (tmpTypName2 == "bool")        //bool type
                                {
                                    BoolVariable &i2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                    bool tmpInt2 = i2.getValue();
                                    BoolVariable *toBeAdded = new BoolVariable (tmpInt2 , name1 , "bool");
                                    initializedVariables.addVariable(*toBeAdded);
                                    BoolVariable *boolTemp = new BoolVariable (tmpInt2 , name1 , "bool");
                                    tempArr.replaceVariable(*boolTemp);
                                }
                                else            //bool array
                                {
                                    BoolArray &Ab2 = (BoolArray &) initializedVariables.getVariable(name2);
                                    int arrLocInt = stringToInt(arrLoc2);
                                    BoolVariable &b2 = (BoolVariable &) (*(Ab2.getBoolElement(arrLocInt)));
                                    BoolVariable *toBeAdded = new BoolVariable(false, name1, "bool");
                                    *toBeAdded = b2;
                                    initializedVariables.addVariable(*toBeAdded);
                                    BoolVariable *boolTemp = new BoolVariable(false, name1, "bool");
                                    *boolTemp = b2;
                                    tempArr.replaceVariable(*boolTemp);
                                }
                            }
                            if (tmpTypName2 == "string" || tmpTypName2 == "stringArray")
                            {
                                if (tmpTypName2 == "string")        //string type
                                {
                                    StringVariable &i2 = (StringVariable &) initializedVariables.getVariable(name2);
                                    string tmpString2 = i2.getValue();
                                    StringVariable *toBeAdded = new StringVariable (tmpString2 , name1 , "string");
                                    initializedVariables.addVariable(*toBeAdded);
                                    StringVariable *stringTemp = new StringVariable (tmpString2 , name1 , "string");
                                    tempArr.replaceVariable(*stringTemp);
                                }
                                else            //string array
                                {
                                    StringArray &As2 = (StringArray &) initializedVariables.getVariable(name2);
                                    int arrLocInt = stringToInt(arrLoc2);
                                    StringVariable &s2 = (StringVariable &) (*(As2.getStringElement(arrLocInt)));
                                    StringVariable *toBeAdded = new StringVariable("", name1, "string");
                                    *toBeAdded = s2;
                                    initializedVariables.addVariable(*toBeAdded);
                                    StringVariable *stringTemp = new StringVariable("", name1, "string");
                                    *stringTemp = s2;
                                    tempArr.replaceVariable(*stringTemp);
                                }
                            }
                        } else {                                     // VAR=VAR
                            // CHECK ITS TYPE AND PUT ITS VALUE INTO THE NEW VARIABLE - true / false / " " / 2 (an integer)
                            // SAVE THE VALUE OF THE NEW VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            // IF IT DOESN'T BELONG TO ANY OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                            if (isNumber(name2)){       //name2 is num
                                int newVal = stringToInt(name2);
                                IntVariable *newIntVal = new  IntVariable(newVal,name1,"int");
                                initializedVariables.addVariable(*newIntVal);
                                IntVariable *intTemp = new IntVariable(newVal,name1,"int");
                                tempArr.replaceVariable(*intTemp);
                            }else
                            if (name2[0] == '"'){       //name2 is string
                                string value = name2.substr(1,name2.length() - 2);
                                StringVariable *newStringVal = new  StringVariable(value,name1,"string");
                                initializedVariables.addVariable(*newStringVal);
                                StringVariable *stringTemp = new  StringVariable(value,name1,"string");
                                tempArr.replaceVariable(*stringTemp);
                            } else
                            if (name2 == "true" || name2 == "false"){   //name2 is bool
                                bool newVal = name2 == "true";
                                BoolVariable *newBoolVal = new  BoolVariable(newVal,name1,"bool");
                                initializedVariables.addVariable(*newBoolVal);
                                BoolVariable *boolTemp = new  BoolVariable(newVal,name1,"bool");
                                tempArr.replaceVariable(*boolTemp);
                            } else
                            if (isArr2) {      //name2 is array
                                int j = 1;
                                int n = 0;
                                while (n < name2.length()) {
                                    if(name2[n] == ',')
                                        j++;
                                    n++;
                                }
                                int firstCellLength = 0;
                                n = 1;
                                while(n < name2.length() && name2[n] != ',') {
                                    firstCellLength++;
                                    n++;
                                }
                                string firstCell = name2.substr(1,(unsigned long)firstCellLength);
                                bool ans = isNumber(firstCell);
                                if (ans){      //int array
                                    int nextCell = 0;
                                    int i = 1;
                                    int len = 0;
                                    int startWord = i;
                                    int arraySize = j;
                                    int tmpArr [arraySize];
                                    while (i < name2.length()) {        //get the whole array
                                        len = 0;
                                        while(i < name2.length() && name2[i] != ']' && name2[i] != ',') {  //get a word
                                            i++;
                                            len++;
                                        }
                                        string value = name2.substr((unsigned long)startWord,len);
                                        int intVal = stringToInt(value);
                                        tmpArr[nextCell] = intVal;
                                        nextCell++;
                                        i = i + 1;
                                        startWord = i;
                                    }
                                    IntArray *newIntArray = new IntArray(tmpArr,arraySize,name1,"intArray");
                                    initializedVariables.addVariable(*newIntArray);
                                }else
                                if (firstCell == "true" || firstCell == "false"){      //bool array
                                    int nextCell = 0;
                                    int i = 1;
                                    int len = 0;
                                    int startWord = i;
                                    int arraySize = j;
                                    bool tmpArr [arraySize];
                                    while (i < name2.length()) {        //get the whole array
                                        len = 0;
                                        while(i < name2.length() && name2[i] != ']' && name2[i] != ',') {  //get a word
                                            i++;
                                            len++;
                                        }
                                        string value = name2.substr((unsigned long)startWord,(unsigned long)len);
                                        bool boolVal = value == "true";
                                        tmpArr[nextCell] = boolVal;
                                        nextCell++;
                                        i = i + 1;
                                        startWord = i;
                                    }
                                    BoolArray *newBoolArray = new BoolArray(tmpArr,arraySize,name1,"boolArray");
                                    initializedVariables.addVariable(*newBoolArray);
                                } else
                                if (name2[1] == '"'){       //string array
                                    int nextCell = 0;
                                    int i = 1;
                                    int len = 0;
                                    int startWord = i;
                                    int arraySize = j;
                                    string tmpArr [arraySize];
                                    while (i < name2.length()) {        //get the whole array
                                        len = 0;
                                        while(i < name2.length() && name2[i] != ']' && name2[i] != ',') {  //get a word
                                            i++;
                                            len++;
                                        }
                                        string value = name2.substr((unsigned long)startWord,(unsigned long)len);
                                        string stringVal = value.substr(1,value.length()-2);
                                        tmpArr[nextCell] = stringVal;
                                        nextCell++;
                                        i = i + 1;
                                        startWord = i;
                                    }
                                    StringArray *newStringArray = new StringArray(tmpArr,arraySize,name1,"stringArray");
                                    initializedVariables.addVariable(*newStringArray);
                                }else
                                    throw BadVariableTypeException(lineNum);
                            } else
                                throw BadVariableTypeException(lineNum);
                        }
                    }
                }
            }
        }
            //IF ITS NOT THE FIRST OPERAND1 OPERATOR OPERAND2 AND NAME IS TEMP
        else {
            // IF ITS A COMMENT
            if (line[columnNum] == '#')
                break;
            // IF THE LAST VAR CALCULATED WAS AN INTEGER
            if (tempArr.getFirstVariable().getType() == "int") {
                IntVariable &prevI1 = (IntVariable&)tempArr.getFirstVariable();
                int nameLength = 0;
                int demiI;
                string name1 = prevI1.getName();
                bool isArr1 = false;
                string arrName1;
                string arrLoc1;
                if (name1.find('[') != std::string::npos) {
                    if(name1[0] == '[') {
                        isArr1 = true;
                    }
                    else {
                        int iter1 = 0;
                        while (name1[iter1] != '[')
                            iter1++;
                        arrName1 = name1.substr(0, (unsigned long) iter1);
                        iter1++;
                        int iter2 = 0;
                        int counter = iter1;
                        while (name1[counter] != ']') {
                            iter2++;
                            counter++;
                        }
                        arrLoc1 = name1.substr((unsigned long) iter1, (unsigned long) iter2);
                        name1 = arrName1;
                        isArr1 = true;
                        if (!(isNumber(arrLoc1))) {
                            if (initializedVariables.doesNameExists(arrLoc1)) {
                                if (initializedVariables.getVariable(arrLoc1).getType() == "int") {
                                    int value = ((IntVariable &) initializedVariables.getVariable(arrLoc1)).getVal();
                                    intToString(value, arrLoc1);
                                } else {
                                    throw BadVariableTypeException(lineNum);
                                }
                            } else {
                                if(arrLoc1.find('+') != std::string::npos || arrLoc1.find('=') != std::string::npos) {
                                    Variable &ans = normalParse(arrLoc1, lineNum, 0, initializedVariables);
                                    if(ans.getType() != "int")
                                        throw BadVariableTypeException(lineNum);
                                    int value = ((IntVariable &)ans).getVal();
                                    delete(&ans);
                                    intToString(value, arrLoc1);
                                } else
                                    throw VariableNotFoundException(lineNum);
                            }
                        }
                    }
                }
                if (line[columnNum] == '+') {                                         // TEMP+
                    if (initializedVariables.doesNameExists(name1)) {               // NAME1+
                        // CHECK IF NAME1 IS TYPE INT, IF NOT THROW BAD VARIABLE TYPE EXCEPTION
                        if (line[columnNum + 1] == '=') {                           // NAME1+=
                            columnNum = columnNum + 2;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long) columnNum, (unsigned long) nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {                  // NAME1+=NAME2
                                // CHECK IF ITS AN INT VARIABLE
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    if (isArr1) {                           // ARR[NAM1]+=ARR[NAME2]
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                        i1 += i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]",
                                                                               "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    } else {                              // NAME1+=ARR[NAME2]
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                        i1 += i2;
                                        IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                } else {
                                    if (isArr1) {                        // ARR[NAME1]+=NAME2
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        i1 += i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]",
                                                                               "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    } else {                                 // NAME1+=NAME2
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        i1 += i2;
                                        IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                        // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                    }
                                }
                            } else {                                           // NAME1+=INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                if (isArr1) {                         // ARR[NAME1]+=INT
                                                    IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                                    int arrLocInt1 = stringToInt(arrLoc1);
                                                    IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                                    i1 += i2;
                                                    IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]", "int");
                                                    *intTemp = i1;
                                                    tempArr.replaceVariable(*intTemp);
                                                } else {                               // NAME1+=INT
                                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(
                                                            name1);
                                                    i1 += i2;
                                                    IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                                    *intTemp = i1;
                                                    tempArr.replaceVariable(*intTemp);
                                                }
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        } else {                         // NAME1+
                            columnNum++;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long) columnNum, (unsigned long) nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {            // NAME1+NAME2
                                // CHECK IF ITS AN INT VARIABLE
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {                       // NAME1+ARR[NAME2]
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    IntVariable &intTemp = prevI1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                } else {                                 // TEMP1+NAME2
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    // IF ITS AN INT, COMBINE THEM AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    IntVariable &intTemp = prevI1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                }
                            } else {                             // NAME1+INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                if (name2[0] == '"') {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '[') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        // IF ITS AN INT VARIABLE COMBINE THEM AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                        if (isNumber(name2)) {
                                            int value = stringToInt(name2);
                                            IntVariable i2 = IntVariable(value, "", "int");
                                            IntVariable &intTemp = prevI1 + i2;
                                            tempArr.replaceVariable(intTemp);
                                        } else {
                                            // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                            throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                        }
                    } else {                                     // TEMP+
                        if (line[columnNum + 1] == '=') {          // TEMP+=
                            columnNum = columnNum + 2;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long) columnNum, (unsigned long) nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {        // TEMP+=NAME2
                                // CHECK IF ITS AN INT VARIABLE
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {                             // TEMP+=ARR[NAME2]
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    prevI1 += i2;
                                    IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                    *intTemp = prevI1;
                                    tempArr.replaceVariable(*intTemp);
                                } else {                                 // TEMP+=NAME2
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    prevI1 += i2;
                                    IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                    *intTemp = prevI1;
                                    tempArr.replaceVariable(*intTemp);
                                }
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            } else {                                         // INT+=INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                prevI1 += i2;
                                                IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                                *intTemp = prevI1;
                                                tempArr.replaceVariable(*intTemp);
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        } else {                // INT+
                            columnNum++;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long) columnNum, (unsigned long) nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {              // INT+NAME2
                                // CHECK IF ITS AN INT VARIABLE
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {                             // INT+ARR[NAME2]
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    IntVariable &intTemp = prevI1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                } else {                                 // INT+NAME2
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                    IntVariable &intTemp = prevI1 + i2;
                                    tempArr.replaceVariable(intTemp);
                                }
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                            } else {                                       // INT+INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                // ELSE, THROW BAD VARIABLE TYPE EXCEPTION
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                IntVariable &intTemp = prevI1 + i2;
                                                tempArr.replaceVariable(intTemp);
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (line[columnNum] == '=') {                         // TEMP=
                    if (initializedVariables.doesNameExists(name1)) {           // NAME1=
                        if (line[columnNum + 1] == '=') {                     // NAME1==
                            // CHECK ITS TYPE AND SAVE IT
                            columnNum = columnNum + 2;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {           // NAME1==NAME2
                                // CHECK IF ITS THE SAME TYPE AS NAME1
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {                          // NAME1==ARR[NAME2]
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    BoolVariable *boolTemp = new BoolVariable(prevI1 == i2,"noName","bool");
                                    tempArr.replaceVariable(*boolTemp);
                                }
                                else {                             // NAME1==NAME2
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    BoolVariable *boolTemp = new BoolVariable(prevI1 == i2,"noName","bool");
                                    tempArr.replaceVariable(*boolTemp);
                                }
                                // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                            } else {                                          // NAME1==INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                BoolVariable *boolTemp = new BoolVariable(prevI1 == i2,"noName","bool");
                                                tempArr.replaceVariable(*boolTemp);
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        } else {                            // NAME1=
                            // CHECK ITS TYPE AND SAVE IT
                            columnNum++;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {          // NAME1=NAME2
                                // CHECK IF ITS THE SAME TYPE AS NAME1
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    if(isArr1) {                                // ARR[NAME1]=ARR[NAME2]
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]", "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                    else {                                     // NAME1=ARR[NAME2]
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                }
                                else {
                                    if(isArr1) {                                // ARR[NAME1]=NAME2
                                        IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                        int arrLocInt1 = stringToInt(arrLoc1);
                                        IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, iA1.getName() + "[" + arrLoc1 + "]", "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                    else {                                     // NAME1=NAME2
                                        IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(name1);
                                        IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                        i1 = i2;
                                        IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                        *intTemp = i1;
                                        tempArr.replaceVariable(*intTemp);
                                    }
                                }
                            } else {                                        // NAME1=VAR
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                if (isArr1) {                         // ARR[NAME1]+=INT
                                                    IntArray &iA1 = (IntArray &) initializedVariables.getVariable(name1);
                                                    int arrLocInt1 = stringToInt(arrLoc1);
                                                    IntVariable &i1 = (IntVariable &) (*iA1.getIntElement(arrLocInt1));
                                                    i1 = i2;
                                                    IntVariable *intTemp = new IntVariable(0,
                                                                                           iA1.getName() + "[" + arrLoc1 +
                                                                                           "]",
                                                                                           "int");
                                                    *intTemp = i1;
                                                    tempArr.replaceVariable(*intTemp);
                                                } else {                               // NAME1+=INT
                                                    IntVariable &i1 = (IntVariable &) initializedVariables.getVariable(
                                                            name1);
                                                    i1 = i2;
                                                    IntVariable *intTemp = new IntVariable(0, i1.getName(), "int");
                                                    *intTemp = i1;
                                                    tempArr.replaceVariable(*intTemp);
                                                }
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } else {                                   // TEMP=
                        if (line[columnNum + 1] == '=') {               // TEMP==
                            // CHECK ITS TYPE AND SAVE IT
                            columnNum = columnNum + 2;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {            // TEMP==NAME2
                                // CHECK IF ITS THE SAME TYPE AS NAME1
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {                              // TEMP==ARR[NAME2]
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    BoolVariable *boolTemp = new BoolVariable(prevI1 == i2,"noName","bool");
                                    tempArr.replaceVariable(*boolTemp);
                                }
                                else {                                    // TEMP==NAME2
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    BoolVariable *boolTemp = new BoolVariable(prevI1 == i2,"noName","bool");
                                    tempArr.replaceVariable(*boolTemp);
                                }
                            } else {                                    // TEMP==INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                BoolVariable *boolTemp = new BoolVariable(prevI1 == i2,"noName","bool");
                                                tempArr.replaceVariable(*boolTemp);
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        } else {                       // TEMP=
                            columnNum++;
                            demiI = columnNum;
                            nameLength = 0;
                            bool isArrCellOp1 = false;
                            while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                    line[demiI] != '}')) {
                                if(line[demiI] == '[')
                                    isArrCellOp1 = true;
                                if(line[demiI] == ']')
                                    isArrCellOp1 = false;
                                nameLength++;
                                demiI++;
                            }
                            string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                            bool isArr2 = false;
                            string arrName2;
                            string arrLoc2;
                            if (name2.find('[') != std::string::npos) {
                                if(name2[0] == '[') {
                                    isArr2 = true;
                                }
                                else {
                                    int iter3 = 0;
                                    while (name2[iter3] != '[')
                                        iter3++;
                                    arrName2 = name2.substr(0, (unsigned long)iter3);
                                    iter3++;
                                    int iter4 = 0;
                                    int counter = iter3;
                                    while (name2[counter] != ']') {
                                        iter4++;
                                        counter++;
                                    }
                                    arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                    name2 = arrName2;
                                    isArr2 = true;
                                    if(!(isNumber(arrLoc2))) {
                                        if(initializedVariables.doesNameExists(arrLoc2)) {
                                            if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                intToString(value,arrLoc2);
                                            }
                                            else {
                                                throw BadVariableTypeException(lineNum);
                                            }
                                        }
                                        else {
                                            if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                if(ans.getType() != "int")
                                                    throw BadVariableTypeException(lineNum);
                                                int value = ((IntVariable &)ans).getVal();
                                                delete(&ans);
                                                intToString(value, arrLoc2);
                                            } else
                                                throw VariableNotFoundException(lineNum);
                                        }
                                    }
                                }
                            }
                            columnNum = demiI;
                            if (initializedVariables.doesNameExists(name2)) {               // TEMP=NAME2
                                // CHECK ITS TYPE AND PUT ITS VALUE INTO THE NEW VARIABLE
                                if (initializedVariables.getVariable(name2).getType() != "int" &&
                                    initializedVariables.getVariable(name2).getType() != "intArray")
                                    throw BadVariableTypeException(lineNum);
                                if (isArr2) {
                                    IntArray &iA2 = (IntArray &) initializedVariables.getVariable(name2);
                                    int arrSize2 = iA2.getSize();
                                    int arrLocInt2 = stringToInt(arrLoc2);
                                    if (arrLocInt2 > arrSize2 - 1)
                                        throw ArrayOutOfBoundException(lineNum);
                                    IntVariable &i2 = (IntVariable &) (*iA2.getIntElement(arrLocInt2));
                                    prevI1 = i2;
                                    IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                    *intTemp = prevI1;
                                    tempArr.replaceVariable(*intTemp);
                                }
                                else {
                                    IntVariable &i2 = (IntVariable &) initializedVariables.getVariable(name2);
                                    prevI1 = i2;
                                    IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                    *intTemp = prevI1;
                                    tempArr.replaceVariable(*intTemp);
                                }
                            } else {                                     // TEMP=INT
                                // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                if (name2 == "true" || name2 == "false") {
                                    throw BadVariableTypeException(lineNum);
                                } else {
                                    if (name2[0] == '"') {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '[') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                            if (isNumber(name2)) {
                                                int value = stringToInt(name2);
                                                IntVariable i2 = IntVariable(value, "", "int");
                                                prevI1 = i2;
                                                IntVariable *intTemp = new IntVariable(0, "noName", "int");
                                                *intTemp = prevI1;
                                                tempArr.replaceVariable(*intTemp);
                                            } else {
                                                // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                // IF THE LAST VAR CALCULATED WAS A BOOLEAN
                if (tempArr.getFirstVariable().getType() == "bool") {
                    BoolVariable &prevB1 = (BoolVariable&)tempArr.getFirstVariable();
                    int nameLength = 0;
                    int demiI;
                    string name1 = prevB1.getName();
                    bool isArr1 = false;
                    string arrName1;
                    string arrLoc1;
                    if (name1.find('[') != std::string::npos) {
                        if(name1[0] == '[') {
                            isArr1 = true;
                        }
                        else {
                            int iter1 = 0;
                            while (name1[iter1] != '[')
                                iter1++;
                            arrName1 = name1.substr(0, (unsigned long)iter1);
                            iter1++;
                            int iter2 = 0;
                            int counter = iter1;
                            while (name1[counter] != ']') {
                                iter2++;
                                counter++;
                            }
                            arrLoc1 = name1.substr((unsigned long)iter1, (unsigned long)iter2);
                            name1 = arrName1;
                            isArr1 = true;
                            if(!(isNumber(arrLoc1))) {
                                if(initializedVariables.doesNameExists(arrLoc1)) {
                                    if(initializedVariables.getVariable(arrLoc1).getType() == "int") {
                                        int value = ((IntVariable&)initializedVariables.getVariable(arrLoc1)).getVal();
                                        intToString(value,arrLoc1);
                                    }
                                    else {
                                        throw BadVariableTypeException(lineNum);
                                    }
                                }
                                else {
                                    if(arrLoc1.find('+') != std::string::npos || arrLoc1.find('=') != std::string::npos) {
                                        Variable &ans = normalParse(arrLoc1, lineNum, 0, initializedVariables);
                                        if(ans.getType() != "int")
                                            throw BadVariableTypeException(lineNum);
                                        int value = ((IntVariable &)ans).getVal();
                                        delete(&ans);
                                        intToString(value, arrLoc1);
                                    } else
                                        throw VariableNotFoundException(lineNum);
                                }
                            }
                        }
                    }
                    if (line[columnNum] == '=') {                         // TEMP=
                        if (initializedVariables.doesNameExists(name1)) {           // NAME1=
                            if (line[columnNum + 1] == '=') {                     // NAME1==
                                // CHECK ITS TYPE AND SAVE IT
                                columnNum = columnNum + 2;
                                demiI = columnNum;
                                nameLength = 0;
                                bool isArrCellOp1 = false;
                                while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                        line[demiI] != '}')) {
                                    if(line[demiI] == '[')
                                        isArrCellOp1 = true;
                                    if(line[demiI] == ']')
                                        isArrCellOp1 = false;
                                    nameLength++;
                                    demiI++;
                                }
                                string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                bool isArr2 = false;
                                string arrName2;
                                string arrLoc2;
                                if (name2.find('[') != std::string::npos) {
                                    if(name2[0] == '[') {
                                        isArr2 = true;
                                    }
                                    else {
                                        int iter3 = 0;
                                        while (name2[iter3] != '[')
                                            iter3++;
                                        arrName2 = name2.substr(0, (unsigned long)iter3);
                                        iter3++;
                                        int iter4 = 0;
                                        int counter = iter3;
                                        while (name2[counter] != ']') {
                                            iter4++;
                                            counter++;
                                        }
                                        arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                        name2 = arrName2;
                                        isArr2 = true;
                                        if(!(isNumber(arrLoc2))) {
                                            if(initializedVariables.doesNameExists(arrLoc2)) {
                                                if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                    int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                    intToString(value,arrLoc2);
                                                }
                                                else {
                                                    throw BadVariableTypeException(lineNum);
                                                }
                                            }
                                            else {
                                                if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                    Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                    if(ans.getType() != "int")
                                                        throw BadVariableTypeException(lineNum);
                                                    int value = ((IntVariable &)ans).getVal();
                                                    delete(&ans);
                                                    intToString(value, arrLoc2);
                                                } else
                                                    throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                                columnNum = demiI;
                                if (initializedVariables.doesNameExists(name2)) {           // NAME1==NAME2
                                    // CHECK IF ITS THE SAME TYPE AS NAME1
                                    if (initializedVariables.getVariable(name2).getType() != "bool" &&
                                        initializedVariables.getVariable(name2).getType() != "boolArray")
                                        throw BadVariableTypeException(lineNum);
                                    if (isArr2) {                          // NAME1==ARR[NAME2]
                                        BoolArray &bA2 = (BoolArray &) initializedVariables.getVariable(name2);
                                        int arrSize2 = bA2.getSize();
                                        int arrLocInt2 = stringToInt(arrLoc2);
                                        if (arrLocInt2 > arrSize2 - 1)
                                            throw ArrayOutOfBoundException(lineNum);
                                        BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                        BoolVariable *boolTemp = new BoolVariable(prevB1 == b2,"noName","bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                    else {                             // NAME1==NAME2
                                        BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(prevB1 == b2,"noName","bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                    // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                                } else {                                          // NAME1==BOOL
                                    // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                    // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                    if (isNumber(name2)) {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '"') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2[0] == '[') {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                // IF ITS A BOOL, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                if (name2 == "true" || name2 == "false") {
                                                    bool value = (name2 == "true");
                                                    BoolVariable b2 = BoolVariable(value, "", "bool");
                                                    BoolVariable *boolTemp = new BoolVariable(prevB1 == b2,"noName","bool");
                                                    tempArr.replaceVariable(*boolTemp);
                                                } else {
                                                    // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                    throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                }
                            } else {                            // NAME1=
                                // CHECK ITS TYPE AND SAVE IT
                                columnNum++;
                                demiI = columnNum;
                                nameLength = 0;
                                bool isArrCellOp1 = false;
                                while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                        line[demiI] != '}')) {
                                    if(line[demiI] == '[')
                                        isArrCellOp1 = true;
                                    if(line[demiI] == ']')
                                        isArrCellOp1 = false;
                                    nameLength++;
                                    demiI++;
                                }
                                string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                bool isArr2 = false;
                                string arrName2;
                                string arrLoc2;
                                if (name2.find('[') != std::string::npos) {
                                    if(name2[0] == '[') {
                                        isArr2 = true;
                                    }
                                    else {
                                        int iter3 = 0;
                                        while (name2[iter3] != '[')
                                            iter3++;
                                        arrName2 = name2.substr(0, (unsigned long)iter3);
                                        iter3++;
                                        int iter4 = 0;
                                        int counter = iter3;
                                        while (name2[counter] != ']') {
                                            iter4++;
                                            counter++;
                                        }
                                        arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                        name2 = arrName2;
                                        isArr2 = true;
                                        if(!(isNumber(arrLoc2))) {
                                            if(initializedVariables.doesNameExists(arrLoc2)) {
                                                if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                    int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                    intToString(value,arrLoc2);
                                                }
                                                else {
                                                    throw BadVariableTypeException(lineNum);
                                                }
                                            }
                                            else {
                                                if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                    Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                    if(ans.getType() != "int")
                                                        throw BadVariableTypeException(lineNum);
                                                    int value = ((IntVariable &)ans).getVal();
                                                    delete(&ans);
                                                    intToString(value, arrLoc2);
                                                } else
                                                    throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                                columnNum = demiI;
                                if (initializedVariables.doesNameExists(name2)) {          // NAME1=NAME2
                                    // CHECK IF ITS THE SAME TYPE AS NAME1
                                    if (initializedVariables.getVariable(name2).getType() != "bool" &&
                                        initializedVariables.getVariable(name2).getType() != "boolArray")
                                        throw BadVariableTypeException(lineNum);
                                    if (isArr2) {
                                        BoolArray &bA2 = (BoolArray &) initializedVariables.getVariable(name2);
                                        int arrSize2 = bA2.getSize();
                                        int arrLocInt2 = stringToInt(arrLoc2);
                                        if (arrLocInt2 > arrSize2 - 1)
                                            throw ArrayOutOfBoundException(lineNum);
                                        if(isArr1) {                                // ARR[NAME1]=ARR[NAME2]
                                            BoolArray bA1 = (BoolArray &) initializedVariables.getVariable(name1);
                                            int arrLocInt1 = stringToInt(arrLoc1);
                                            BoolVariable &b1 = (BoolVariable &) (*bA1.getBoolElement(arrLocInt1));
                                            BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                            b1 = b2;
                                            BoolVariable *boolTemp = new BoolVariable(false, bA1.getName() + "[" + arrLoc1 + "]", "bool");
                                            *boolTemp = b1;
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                        else {                                     // NAME1=ARR[NAME2]
                                            BoolVariable &b1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                            BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                            b1 = b2;
                                            BoolVariable *boolTemp = new BoolVariable(false, b1.getName(), "bool");
                                            *boolTemp = b1;
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                    }
                                    else {
                                        if(isArr1) {                                // ARR[NAME1]=NAME2
                                            BoolArray &bA1 = (BoolArray &) initializedVariables.getVariable(name1);
                                            int arrLocInt1 = stringToInt(arrLoc1);
                                            BoolVariable &b1 = (BoolVariable &) (*bA1.getBoolElement(arrLocInt1));
                                            BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                            b1 = b2;
                                            BoolVariable *boolTemp = new BoolVariable(false, bA1.getName() + "[" + arrLoc1 + "]", "bool");
                                            *boolTemp = b1;
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                        else {                                     // NAME1=NAME2
                                            BoolVariable &b1 = (BoolVariable &) initializedVariables.getVariable(name1);
                                            BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                            b1 = b2;
                                            BoolVariable *boolTemp = new BoolVariable(false, b1.getName(), "bool");
                                            *boolTemp = b1;
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                    }
                                } else {                                        // NAME1=BOOL
                                    // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                    if (isNumber(name2)) {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '"') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2[0] == '[') {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                // IF ITS A BOOL, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                if (name2 == "true" || name2 == "false") {
                                                    bool value = (name2 == "true");
                                                    BoolVariable b2 = BoolVariable(value, "", "bool");
                                                    if (isArr1) {                         // ARR[NAME1]=BOOL
                                                        BoolArray &bA1 = (BoolArray &) initializedVariables.getVariable(name1);
                                                        int arrLocInt1 = stringToInt(arrLoc1);
                                                        BoolVariable &b1 = (BoolVariable &) (*bA1.getBoolElement(arrLocInt1));
                                                        b1 = b2;
                                                        BoolVariable *boolTemp = new BoolVariable(false,
                                                                                                  bA1.getName() + "[" + arrLoc1 +
                                                                                                  "]",
                                                                                                  "bool");
                                                        *boolTemp = b1;
                                                        tempArr.replaceVariable(*boolTemp);
                                                    } else {                               // NAME1=BOOL
                                                        BoolVariable &b1 = (BoolVariable &) initializedVariables.getVariable(
                                                                name1);
                                                        b1 = b2;
                                                        BoolVariable *boolTemp = new BoolVariable(false, b1.getName(), "bool");
                                                        *boolTemp = b1;
                                                        tempArr.replaceVariable(*boolTemp);
                                                    }
                                                } else {
                                                    // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                    throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else {                                   // TEMP=
                            if (line[columnNum + 1] == '=') {               // TEMP==
                                // CHECK ITS TYPE AND SAVE IT
                                columnNum = columnNum + 2;
                                demiI = columnNum;
                                nameLength = 0;
                                bool isArrCellOp1 = false;
                                while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                        line[demiI] != '}')) {
                                    if(line[demiI] == '[')
                                        isArrCellOp1 = true;
                                    if(line[demiI] == ']')
                                        isArrCellOp1 = false;
                                    nameLength++;
                                    demiI++;
                                }
                                string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                bool isArr2 = false;
                                string arrName2;
                                string arrLoc2;
                                if (name2.find('[') != std::string::npos) {
                                    if(name2[0] == '[') {
                                        isArr2 = true;
                                    }
                                    else {
                                        int iter3 = 0;
                                        while (name2[iter3] != '[')
                                            iter3++;
                                        arrName2 = name2.substr(0, (unsigned long)iter3);
                                        iter3++;
                                        int iter4 = 0;
                                        int counter = iter3;
                                        while (name2[counter] != ']') {
                                            iter4++;
                                            counter++;
                                        }
                                        arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                        name2 = arrName2;
                                        isArr2 = true;
                                        if(!(isNumber(arrLoc2))) {
                                            if(initializedVariables.doesNameExists(arrLoc2)) {
                                                if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                    int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                    intToString(value,arrLoc2);
                                                }
                                                else {
                                                    throw BadVariableTypeException(lineNum);
                                                }
                                            }
                                            else {
                                                if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                    Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                    if(ans.getType() != "int")
                                                        throw BadVariableTypeException(lineNum);
                                                    int value = ((IntVariable &)ans).getVal();
                                                    delete(&ans);
                                                    intToString(value, arrLoc2);
                                                } else
                                                    throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                                columnNum = demiI;
                                if (initializedVariables.doesNameExists(name2)) {            // TEMP==NAME2
                                    // CHECK IF ITS THE SAME TYPE AS NAME1
                                    if (initializedVariables.getVariable(name2).getType() != "bool" &&
                                        initializedVariables.getVariable(name2).getType() != "boolArray")
                                        throw BadVariableTypeException(lineNum);
                                    if (isArr2) {                              // TEMP==ARR[NAME2]
                                        BoolArray &bA2 = (BoolArray &) initializedVariables.getVariable(name2);
                                        int arrSize2 = bA2.getSize();
                                        int arrLocInt2 = stringToInt(arrLoc2);
                                        if (arrLocInt2 > arrSize2 - 1)
                                            throw ArrayOutOfBoundException(lineNum);
                                        BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                        BoolVariable *boolTemp = new BoolVariable(prevB1 == b2,"noName","bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                    else {                                    // TEMP==NAME2
                                        BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        BoolVariable *boolTemp = new BoolVariable(prevB1 == b2,"noName","bool");
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                } else {                                    // TEMP==BOOL
                                    // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                    // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                    if (isNumber(name2)) {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '"') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2[0] == '[') {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                // IF ITS A BOOL, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                if (name2 == "true" || name2 == "false") {
                                                    bool value = (name2 == "true");
                                                    BoolVariable b2 = BoolVariable(value, "", "bool");
                                                    BoolVariable *boolTemp = new BoolVariable(prevB1 == b2,"noName","bool");
                                                    tempArr.replaceVariable(*boolTemp);
                                                } else {
                                                    // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                    throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                }
                            } else {                       // TEMP=
                                columnNum++;
                                demiI = columnNum;
                                nameLength = 0;
                                bool isArrCellOp1 = false;
                                while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                        line[demiI] != '}')) {
                                    if(line[demiI] == '[')
                                        isArrCellOp1 = true;
                                    if(line[demiI] == ']')
                                        isArrCellOp1 = false;
                                    nameLength++;
                                    demiI++;
                                }
                                string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                bool isArr2 = false;
                                string arrName2;
                                string arrLoc2;
                                if (name2.find('[') != std::string::npos) {
                                    if(name2[0] == '[') {
                                        isArr2 = true;
                                    }
                                    else {
                                        int iter3 = 0;
                                        while (name2[iter3] != '[')
                                            iter3++;
                                        arrName2 = name2.substr(0, (unsigned long)iter3);
                                        iter3++;
                                        int iter4 = 0;
                                        int counter = iter3;
                                        while (name2[counter] != ']') {
                                            iter4++;
                                            counter++;
                                        }
                                        arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                        name2 = arrName2;
                                        isArr2 = true;
                                        if(!(isNumber(arrLoc2))) {
                                            if(initializedVariables.doesNameExists(arrLoc2)) {
                                                if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                    int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                    intToString(value,arrLoc2);
                                                }
                                                else {
                                                    throw BadVariableTypeException(lineNum);
                                                }
                                            }
                                            else {
                                                if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                    Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                    if(ans.getType() != "int")
                                                        throw BadVariableTypeException(lineNum);
                                                    int value = ((IntVariable &)ans).getVal();
                                                    delete(&ans);
                                                    intToString(value, arrLoc2);
                                                } else
                                                    throw VariableNotFoundException(lineNum);
                                            }
                                        }
                                    }
                                }
                                columnNum = demiI;
                                if (initializedVariables.doesNameExists(name2)) {               // TEMP=NAME2
                                    // CHECK ITS TYPE AND PUT ITS VALUE INTO THE NEW VARIABLE
                                    if (initializedVariables.getVariable(name2).getType() != "bool" &&
                                        initializedVariables.getVariable(name2).getType() != "boolArray")
                                        throw BadVariableTypeException(lineNum);
                                    if (isArr2) {                                 // TEMP=ARR[NAME2]
                                        BoolArray &bA2 = (BoolArray &) initializedVariables.getVariable(name2);
                                        int arrSize2 = bA2.getSize();
                                        int arrLocInt2 = stringToInt(arrLoc2);
                                        if (arrLocInt2 > arrSize2 - 1)
                                            throw ArrayOutOfBoundException(lineNum);
                                        BoolVariable &b2 = (BoolVariable &) (*bA2.getBoolElement(arrLocInt2));
                                        prevB1 = b2;
                                        BoolVariable *boolTemp = new BoolVariable(false, "noName", "bool");
                                        *boolTemp = prevB1;
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                    else {                                       // TEMP=NAME2
                                        BoolVariable &b2 = (BoolVariable &) initializedVariables.getVariable(name2);
                                        prevB1 = b2;
                                        BoolVariable *boolTemp = new BoolVariable(false, "noName", "bool");
                                        *boolTemp = prevB1;
                                        tempArr.replaceVariable(*boolTemp);
                                    }
                                } else {                                     // TEMP=BOOL
                                    // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                    if (isNumber(name2)) {
                                        throw BadVariableTypeException(lineNum);
                                    } else {
                                        if (name2[0] == '"') {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2[0] == '[') {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                // IF ITS A BOOL, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                if (name2 == "true" || name2 == "false") {
                                                    bool value = (name2 == "true");
                                                    BoolVariable b2 = BoolVariable(value, "", "bool");
                                                    prevB1 = b2;
                                                    BoolVariable *boolTemp = new BoolVariable(false, "noName", "bool");
                                                    *boolTemp = prevB1;
                                                    tempArr.replaceVariable(*boolTemp);
                                                } else {
                                                    // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                    throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                        // IF THE OPERATOR ISN'T '=' / '==' THAN A BOOL VARIABLE CAN'T BE CALCULATED WITH IT
                    else
                        throw BadVariableTypeException(lineNum);
                }
                else
                {
                    // IF THE LAST VAR CALCULATED WAS A STRING
                    if (tempArr.getFirstVariable().getType() == "string") {
                        StringVariable &prevS1 = (StringVariable&)tempArr.getFirstVariable();
                        int nameLength = 0;
                        int demiI;
                        string name1 = prevS1.getName();
                        bool isArr1 = false;
                        string arrName1;
                        string arrLoc1;
                        if (name1.find('[') != std::string::npos) {
                            if(name1[0] == '[') {
                                isArr1 = true;
                            }
                            else {
                                int iter1 = 0;
                                while (name1[iter1] != '[')
                                    iter1++;
                                arrName1 = name1.substr(0, (unsigned long)iter1);
                                iter1++;
                                int iter2 = 0;
                                int counter = iter1;
                                while (name1[counter] != ']') {
                                    iter2++;
                                    counter++;
                                }
                                arrLoc1 = name1.substr((unsigned long)iter1, (unsigned long)iter2);
                                name1 = arrName1;
                                isArr1 = true;
                                if(!(isNumber(arrLoc1))) {
                                    if(initializedVariables.doesNameExists(arrLoc1)) {
                                        if(initializedVariables.getVariable(arrLoc1).getType() == "int") {
                                            int value = ((IntVariable&)initializedVariables.getVariable(arrLoc1)).getVal();
                                            intToString(value,arrLoc1);
                                        }
                                        else {
                                            throw BadVariableTypeException(lineNum);
                                        }
                                    }
                                    else {
                                        if(arrLoc1.find('+') != std::string::npos || arrLoc1.find('=') != std::string::npos) {
                                            Variable &ans = normalParse(arrLoc1, lineNum, 0, initializedVariables);
                                            if(ans.getType() != "int")
                                                throw BadVariableTypeException(lineNum);
                                            int value = ((IntVariable &)ans).getVal();
                                            delete(&ans);
                                            intToString(value, arrLoc1);
                                        } else
                                            throw VariableNotFoundException(lineNum);
                                    }
                                }
                            }
                        }
                        if (line[columnNum] == '=') {                         // TEMP=
                            if (initializedVariables.doesNameExists(name1)) {           // NAME1=
                                if (line[columnNum + 1] == '=') {                     // NAME1==
                                    // CHECK ITS TYPE AND SAVE IT
                                    columnNum = columnNum + 2;
                                    demiI = columnNum;
                                    nameLength = 0;
                                    bool isArrCellOp1 = false;
                                    while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                            line[demiI] != '}')) {
                                        if(line[demiI] == '[')
                                            isArrCellOp1 = true;
                                        if(line[demiI] == ']')
                                            isArrCellOp1 = false;
                                        nameLength++;
                                        demiI++;
                                    }
                                    string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                    bool isArr2 = false;
                                    string arrName2;
                                    string arrLoc2;
                                    if (name2.find('[') != std::string::npos) {
                                        if(name2[0] == '[') {
                                            isArr2 = true;
                                        }
                                        else {
                                            int iter3 = 0;
                                            while (name2[iter3] != '[')
                                                iter3++;
                                            arrName2 = name2.substr(0, (unsigned long)iter3);
                                            iter3++;
                                            int iter4 = 0;
                                            int counter = iter3;
                                            while (name2[counter] != ']') {
                                                iter4++;
                                                counter++;
                                            }
                                            arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                            name2 = arrName2;
                                            isArr2 = true;
                                            if(!(isNumber(arrLoc2))) {
                                                if(initializedVariables.doesNameExists(arrLoc2)) {
                                                    if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                        int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                        intToString(value,arrLoc2);
                                                    }
                                                    else {
                                                        throw BadVariableTypeException(lineNum);
                                                    }
                                                }
                                                else {
                                                    if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                        Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                        if(ans.getType() != "int")
                                                            throw BadVariableTypeException(lineNum);
                                                        int value = ((IntVariable &)ans).getVal();
                                                        delete(&ans);
                                                        intToString(value, arrLoc2);
                                                    } else
                                                        throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                    columnNum = demiI;
                                    if (initializedVariables.doesNameExists(name2)) {           // NAME1==NAME2
                                        // CHECK IF ITS THE SAME TYPE AS NAME1
                                        if (initializedVariables.getVariable(name2).getType() != "string" &&
                                            initializedVariables.getVariable(name2).getType() != "stringArray")
                                            throw BadVariableTypeException(lineNum);
                                        if (isArr2) {                          // NAME1==ARR[NAME2]
                                            StringArray &sA2 = (StringArray &) initializedVariables.getVariable(name2);
                                            int arrSize2 = sA2.getSize();
                                            int arrLocInt2 = stringToInt(arrLoc2);
                                            if (arrLocInt2 > arrSize2 - 1)
                                                throw ArrayOutOfBoundException(lineNum);
                                            StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                            BoolVariable *boolTemp = new BoolVariable(prevS1 == s2,"noName","bool");
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                        else {                             // NAME1==NAME2
                                            StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                            BoolVariable *boolTemp = new BoolVariable(prevS1 == s2,"noName","bool");
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                        // IF YES, CHECK IF THE VALUES ARE THE SAME AND AND SAVE THE VALUE (TRUE/FALSE) VARIABLE FOR A POSSIBLE NEXT OPERATOR
                                    } else {                                          // NAME1==STRING
                                        // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                        if (isNumber(name2)) {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2 == "true" || name2 == "false") {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                if (name2[0] == '[') {
                                                    throw BadVariableTypeException(lineNum);
                                                } else {
                                                    // IF ITS A STRING, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                    if (name2[0] == '"') {
                                                        string value = name2.substr(1,name2.length() - 2);
                                                        StringVariable s2 = StringVariable(value, "", "string");
                                                        BoolVariable *boolTemp = new BoolVariable(prevS1 == s2,"noName","bool");
                                                        tempArr.replaceVariable(*boolTemp);
                                                    } else {
                                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                        throw VariableNotFoundException(lineNum);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                } else {                            // NAME1=
                                    // CHECK ITS TYPE AND SAVE IT
                                    columnNum++;
                                    demiI = columnNum;
                                    nameLength = 0;
                                    bool isArrCellOp1 = false;
                                    while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                            line[demiI] != '}')) {
                                        if(line[demiI] == '[')
                                            isArrCellOp1 = true;
                                        if(line[demiI] == ']')
                                            isArrCellOp1 = false;
                                        nameLength++;
                                        demiI++;
                                    }
                                    string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                    bool isArr2 = false;
                                    string arrName2;
                                    string arrLoc2;
                                    if (name2.find('[') != std::string::npos) {
                                        if(name2[0] == '[') {
                                            isArr2 = true;
                                        }
                                        else {
                                            int iter3 = 0;
                                            while (name2[iter3] != '[')
                                                iter3++;
                                            arrName2 = name2.substr(0, (unsigned long)iter3);
                                            iter3++;
                                            int iter4 = 0;
                                            int counter = iter3;
                                            while (name2[counter] != ']') {
                                                iter4++;
                                                counter++;
                                            }
                                            arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                            name2 = arrName2;
                                            isArr2 = true;
                                            if(!(isNumber(arrLoc2))) {
                                                if(initializedVariables.doesNameExists(arrLoc2)) {
                                                    if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                        int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                        intToString(value,arrLoc2);
                                                    }
                                                    else {
                                                        throw BadVariableTypeException(lineNum);
                                                    }
                                                }
                                                else {
                                                    if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                        Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                        if(ans.getType() != "int")
                                                            throw BadVariableTypeException(lineNum);
                                                        int value = ((IntVariable &)ans).getVal();
                                                        delete(&ans);
                                                        intToString(value, arrLoc2);
                                                    } else
                                                        throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                    columnNum = demiI;
                                    if (initializedVariables.doesNameExists(name2)) {          // NAME1=NAME2
                                        // CHECK IF ITS THE SAME TYPE AS NAME1
                                        if (initializedVariables.getVariable(name2).getType() != "string" &&
                                            initializedVariables.getVariable(name2).getType() != "stringArray")
                                            throw BadVariableTypeException(lineNum);
                                        if (isArr2) {
                                            StringArray &sA2 = (StringArray &) initializedVariables.getVariable(name2);
                                            int arrSize2 = sA2.getSize();
                                            int arrLocInt2 = stringToInt(arrLoc2);
                                            if (arrLocInt2 > arrSize2 - 1)
                                                throw ArrayOutOfBoundException(lineNum);
                                            if(isArr1) {                                // ARR[NAME1]=ARR[NAME2]
                                                StringArray &sA1 = (StringArray &) initializedVariables.getVariable(name1);
                                                int arrLocInt1 = stringToInt(arrLoc1);
                                                StringVariable &s1 = (StringVariable &) (*sA1.getStringElement(arrLocInt1));
                                                StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                                s1 = s2;
                                                StringVariable *stringTemp = new StringVariable("", sA1.getName() + "[" + arrLoc1 + "]", "string");
                                                *stringTemp = s1;
                                                tempArr.replaceVariable(*stringTemp);
                                            }
                                            else {                                     // NAME1=ARR[NAME2]
                                                StringVariable &s1 = (StringVariable &) initializedVariables.getVariable(name1);
                                                StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                                s1 = s2;
                                                StringVariable *stringTemp = new StringVariable("", s1.getName(), "string");
                                                *stringTemp = s1;
                                                tempArr.replaceVariable(*stringTemp);
                                            }
                                        }
                                        else {
                                            if(isArr1) {                                // ARR[NAME1]=NAME2
                                                StringArray &sA1 = (StringArray &) initializedVariables.getVariable(name1);
                                                int arrLocInt1 = stringToInt(arrLoc1);
                                                StringVariable &s1 = (StringVariable &) (*sA1.getStringElement(arrLocInt1));
                                                StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                                s1 = s2;
                                                StringVariable *stringTemp = new StringVariable("", sA1.getName() + "[" + arrLoc1 + "]", "string");
                                                *stringTemp = s1;
                                                tempArr.replaceVariable(*stringTemp);
                                            }
                                            else {                                     // NAME1=NAME2
                                                StringVariable &s1 = (StringVariable &) initializedVariables.getVariable(name1);
                                                StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                                s1 = s2;
                                                StringVariable *stringTemp = new StringVariable("", s1.getName(), "string");
                                                *stringTemp = s1;
                                                tempArr.replaceVariable(*stringTemp);
                                            }
                                        }
                                    } else {                                        // NAME1=STRING
                                        // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                        if (isNumber(name2)) {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2 == "true" || name2 == "false") {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                if (name2[0] == '[') {
                                                    throw BadVariableTypeException(lineNum);
                                                } else {
                                                    // IF ITS AN INT, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                    if (name2[0] == '"') {
                                                        string value = name2.substr(1,name2.length() - 2);
                                                        StringVariable b2 = StringVariable(value, "", "string");
                                                        if (isArr1) {                         // ARR[NAME1]=STRING
                                                            StringArray &sA1 = (StringArray &) initializedVariables.getVariable(name1);
                                                            int arrLocInt1 = stringToInt(arrLoc1);
                                                            StringVariable &s1 = (StringVariable &) (*sA1.getStringElement(arrLocInt1));
                                                            s1 = b2;
                                                            StringVariable *stringTemp = new StringVariable("",
                                                                                                            sA1.getName() + "[" + arrLoc1 +
                                                                                                            "]",
                                                                                                            "string");
                                                            *stringTemp = s1;
                                                            tempArr.replaceVariable(*stringTemp);
                                                        } else {                               // NAME1=STRING
                                                            StringVariable &s1 = (StringVariable &) initializedVariables.getVariable(
                                                                    name1);
                                                            s1 = b2;
                                                            StringVariable *stringTemp = new StringVariable("", s1.getName(), "string");
                                                            *stringTemp = s1;
                                                            tempArr.replaceVariable(*stringTemp);
                                                        }
                                                    } else {
                                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                        throw VariableNotFoundException(lineNum);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            } else {                                   // TEMP=
                                if (line[columnNum + 1] == '=') {               // TEMP==
                                    // CHECK ITS TYPE AND SAVE IT
                                    columnNum = columnNum + 2;
                                    demiI = columnNum;
                                    nameLength = 0;
                                    bool isArrCellOp1 = false;
                                    while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                            line[demiI] != '}')) {
                                        if(line[demiI] == '[')
                                            isArrCellOp1 = true;
                                        if(line[demiI] == ']')
                                            isArrCellOp1 = false;
                                        nameLength++;
                                        demiI++;
                                    }
                                    string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                    bool isArr2 = false;
                                    string arrName2;
                                    string arrLoc2;
                                    if (name2.find('[') != std::string::npos) {
                                        if(name2[0] == '[') {
                                            isArr2 = true;
                                        }
                                        else {
                                            int iter3 = 0;
                                            while (name2[iter3] != '[')
                                                iter3++;
                                            arrName2 = name2.substr(0, (unsigned long)iter3);
                                            iter3++;
                                            int iter4 = 0;
                                            int counter = iter3;
                                            while (name2[counter] != ']') {
                                                iter4++;
                                                counter++;
                                            }
                                            arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                            name2 = arrName2;
                                            isArr2 = true;
                                            if(!(isNumber(arrLoc2))) {
                                                if(initializedVariables.doesNameExists(arrLoc2)) {
                                                    if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                        int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                        intToString(value,arrLoc2);
                                                    }
                                                    else {
                                                        throw BadVariableTypeException(lineNum);
                                                    }
                                                }
                                                else {
                                                    if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                        Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                        if(ans.getType() != "int")
                                                            throw BadVariableTypeException(lineNum);
                                                        int value = ((IntVariable &)ans).getVal();
                                                        delete(&ans);
                                                        intToString(value, arrLoc2);
                                                    } else
                                                        throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                    columnNum = demiI;
                                    if (initializedVariables.doesNameExists(name2)) {            // TEMP==NAME2
                                        // CHECK IF ITS THE SAME TYPE AS NAME1
                                        if (initializedVariables.getVariable(name2).getType() != "string" &&
                                            initializedVariables.getVariable(name2).getType() != "stringArray")
                                            throw BadVariableTypeException(lineNum);
                                        if (isArr2) {                              // TEMP==ARR[NAME2]
                                            StringArray &sA2 = (StringArray &) initializedVariables.getVariable(name2);
                                            int arrSize2 = sA2.getSize();
                                            int arrLocInt2 = stringToInt(arrLoc2);
                                            if (arrLocInt2 > arrSize2 - 1)
                                                throw ArrayOutOfBoundException(lineNum);
                                            StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                            BoolVariable *boolTemp = new BoolVariable(prevS1 == s2,"noName","bool");
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                        else {                                    // TEMP==NAME2
                                            StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                            BoolVariable *boolTemp = new BoolVariable(prevS1 == s2,"noName","bool");
                                            tempArr.replaceVariable(*boolTemp);
                                        }
                                    } else {                                    // TEMP==STRING
                                        // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                        if (isNumber(name2)) {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2 == "true" || name2 == "false") {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                if (name2[0] == '[') {
                                                    throw BadVariableTypeException(lineNum);
                                                } else {
                                                    // IF ITS AN INT, SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                    if (name2[0] == '"') {
                                                        string value = name2.substr(1,name2.length() - 2);
                                                        StringVariable s2 = StringVariable(value, "", "string");
                                                        BoolVariable *boolTemp = new BoolVariable(prevS1 == s2,"noName","bool");
                                                        tempArr.replaceVariable(*boolTemp);
                                                    } else {
                                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                        throw VariableNotFoundException(lineNum);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                } else {                       // TEMP=
                                    columnNum++;
                                    demiI = columnNum;
                                    nameLength = 0;
                                    bool isArrCellOp1 = false;
                                    while (isArrCellOp1 || (demiI < line.length() && line[demiI] != '=' && line[demiI] != '+' && line[demiI] != ')' &&
                                                            line[demiI] != '}')) {
                                        if(line[demiI] == '[')
                                            isArrCellOp1 = true;
                                        if(line[demiI] == ']')
                                            isArrCellOp1 = false;
                                        nameLength++;
                                        demiI++;
                                    }
                                    string name2 = line.substr((unsigned long)columnNum, (unsigned long)nameLength);
                                    bool isArr2 = false;
                                    string arrName2;
                                    string arrLoc2;
                                    if (name2.find('[') != std::string::npos) {
                                        if(name2[0] == '[') {
                                            isArr2 = true;
                                        }
                                        else {
                                            int iter3 = 0;
                                            while (name2[iter3] != '[')
                                                iter3++;
                                            arrName2 = name2.substr(0, (unsigned long)iter3);
                                            iter3++;
                                            int iter4 = 0;
                                            int counter = iter3;
                                            while (name2[counter] != ']') {
                                                iter4++;
                                                counter++;
                                            }
                                            arrLoc2 = name2.substr((unsigned long)iter3, (unsigned long)iter4);
                                            name2 = arrName2;
                                            isArr2 = true;
                                            if(!(isNumber(arrLoc2))) {
                                                if(initializedVariables.doesNameExists(arrLoc2)) {
                                                    if(initializedVariables.getVariable(arrLoc2).getType() == "int") {
                                                        int value = ((IntVariable&)initializedVariables.getVariable(arrLoc2)).getVal();
                                                        intToString(value,arrLoc2);
                                                    }
                                                    else {
                                                        throw BadVariableTypeException(lineNum);
                                                    }
                                                }
                                                else {
                                                    if(arrLoc2.find('+') != std::string::npos || arrLoc2.find('=') != std::string::npos) {
                                                        Variable &ans = normalParse(arrLoc2, lineNum, 0, initializedVariables);
                                                        if(ans.getType() != "int")
                                                            throw BadVariableTypeException(lineNum);
                                                        int value = ((IntVariable &)ans).getVal();
                                                        delete(&ans);
                                                        intToString(value, arrLoc2);
                                                    } else
                                                        throw VariableNotFoundException(lineNum);
                                                }
                                            }
                                        }
                                    }
                                    columnNum = demiI;
                                    if (initializedVariables.doesNameExists(name2)) {               // TEMP=NAME2
                                        // CHECK ITS TYPE AND PUT ITS VALUE INTO THE NEW VARIABLE
                                        if (initializedVariables.getVariable(name2).getType() != "string" &&
                                            initializedVariables.getVariable(name2).getType() != "stringArray")
                                            throw BadVariableTypeException(lineNum);
                                        if (isArr2) {                                 // TEMP=ARR[NAME2]
                                            StringArray &sA2 = (StringArray &) initializedVariables.getVariable(name2);
                                            int arrSize2 = sA2.getSize();
                                            int arrLocInt2 = stringToInt(arrLoc2);
                                            if (arrLocInt2 > arrSize2 - 1)
                                                throw ArrayOutOfBoundException(lineNum);
                                            StringVariable &s2 = (StringVariable &) (*sA2.getStringElement(arrLocInt2));
                                            prevS1 = s2;
                                            StringVariable *stringTemp = new StringVariable("", "noName", "string");
                                            *stringTemp = prevS1;
                                            tempArr.replaceVariable(*stringTemp);
                                        }
                                        else {                                       // TEMP=NAME2
                                            StringVariable &s2 = (StringVariable &) initializedVariables.getVariable(name2);
                                            prevS1 = s2;
                                            StringVariable *stringTemp = new StringVariable("", "noName", "string");
                                            *stringTemp = prevS1;
                                            tempArr.replaceVariable(*stringTemp);
                                        }
                                    } else {                                     // TEMP=STRING
                                        // CHECK ITS TYPE - true / false / " " / 2 (an integer) / [ , ]
                                        if (isNumber(name2)) {
                                            throw BadVariableTypeException(lineNum);
                                        } else {
                                            if (name2 == "true" || name2 == "false") {
                                                throw BadVariableTypeException(lineNum);
                                            } else {
                                                if (name2[0] == '[') {
                                                    throw BadVariableTypeException(lineNum);
                                                } else {
                                                    // IF ITS A STRING, PUT THE NEW VALUE IN NAME1 AND SAVE THE VALUE FOR A POSSIBLE NEXT OPERATOR
                                                    if (name2[0] == '"') {
                                                        string value = name2.substr(1,name2.length() - 2);
                                                        StringVariable s2 = StringVariable(value, "", "string");
                                                        prevS1 = s2;
                                                        StringVariable *stringTemp = new StringVariable("", "noName", "string");
                                                        *stringTemp = prevS1;
                                                        tempArr.replaceVariable(*stringTemp);
                                                    } else {
                                                        // IF ITS NONE OF THE ABOVE, THROW VARIABLE NOT FOUND EXCEPTION
                                                        throw VariableNotFoundException(lineNum);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                            // IF THE OPERATOR ISN'T '=' / '==' THAN A STRING VARIABLE CAN'T BE CALCULATED WITH IT
                        else
                            throw BadVariableTypeException(lineNum);
                    }
                }
            }
        }
    }
    string type = tempArr.getFirstVariable().getType();
    if(type == "bool") {
        BoolVariable *ans = new BoolVariable(((BoolVariable&)tempArr.getFirstVariable()).getValue(),tempArr.getFirstVariable().getName(),"bool");
        delete((&tempArr.getFirstVariable()));
        return *ans;
    }
    if(type == "int") {
        IntVariable *ans = new IntVariable(((IntVariable&)tempArr.getFirstVariable()).getVal(),tempArr.getFirstVariable().getName(),"int");
        delete((&tempArr.getFirstVariable()));
        return *ans;
    }
    if(type == "string") {
        StringVariable *ans = new StringVariable(((StringVariable&)tempArr.getFirstVariable()).getValue(),tempArr.getFirstVariable().getName(),"string");
        delete((&tempArr.getFirstVariable()));
        return *ans;
    }
    if(type == "variable") {
        IntVariable *ans = new IntVariable(0,"ans","variable");
        *ans = (IntVariable&)tempArr.getFirstVariable();
        return *ans;
    }
}


/*
 * printing a given expression which is inside the given line.
 * checking for correct using of operators and variables that need to be printerd.
 * if the line is illegal, an error will be thrown.
 * line - the given line need to be printed.
 * lineNum - the number of the current line.
 * initializedVariables - the current initialized Variables.
 */
void printParse(const string& line,int lineNum,InitializedVariables &initializedVariables)
{
    string toPrint;
    if(line.find(")#") != std::string::npos) {
        unsigned long commentLoc = line.find(")#");
        commentLoc++;
        toPrint = line.substr(6, line.length() - 7 - (line.length() - commentLoc));
    }
    else {
        toPrint = line.substr(6, line.length() - 7);
    }
    bool isRightFromOp = (toPrint.find("]+") != std::string::npos) || (toPrint.find("]=") != std::string::npos);
    bool isComma = toPrint.find(',') != std::string::npos;
    int nameLoc = (int)toPrint.find('[');
    int backCount = nameLoc;
    if(nameLoc != std::string::npos && !isRightFromOp && !isComma)
        while(backCount > 0 && toPrint[backCount] != '+' && toPrint[backCount] != '=') {
            backCount--;
        }
    bool isLeftFromOp = false;
    if(nameLoc != std::string::npos && !isRightFromOp && !isComma)
        isLeftFromOp = (toPrint[backCount] == '+' || toPrint[backCount] == '=');
    if(((toPrint.find('+') != std::string::npos || toPrint.find('=') != std::string::npos) && !(toPrint.find('[') != std::string::npos)) || ((toPrint.find('[') != std::string::npos) && isRightFromOp) || ((toPrint.find('[') != std::string::npos) && isLeftFromOp)) {
        Variable *ans = &normalParse(toPrint, lineNum, 0, initializedVariables);
        if (ans->getType() == "bool")
            cout << *((BoolVariable *) ans) << endl;
        if (ans->getType() == "int")
            cout << *((IntVariable *) ans) << endl;
        if (ans->getType() == "string")
            cout << *((StringVariable *) ans) << endl;
        delete(ans);
    }
    else
    if (isNumber(toPrint)){       //toPrint is num
        int tmpIntVal = stringToInt(toPrint);
        IntVariable newIntVal = IntVariable(tmpIntVal,"","int");
        cout << newIntVal << endl;
    }else
    if (toPrint[0] == '"'){       //name2 is string
        string tmpStringV = toPrint.substr(1,toPrint.length()-2);
        StringVariable newStringV = StringVariable(tmpStringV,"","string");
        cout << newStringV << endl;
    } else
    if (toPrint == "true" || toPrint == "false"){   //name2 is bool
        bool tmpBoolV = toPrint == "true";
        BoolVariable newBoolV = BoolVariable(tmpBoolV,"","bool");
        cout << newBoolV << endl;
    }else
    if (toPrint[0] == '[') {      //toPrint is array
        int j = 1;
        int n = 0;
        while (n < toPrint.length()) {
            if(toPrint[n] == ',')
                j++;
            n++;
        }
        int firstCellLength = 0;
        n = 1;
        while(n < toPrint.length() && toPrint[n] != ',') {
            firstCellLength++;
            n++;
        }
        string firstCell = toPrint.substr(1,(unsigned long)firstCellLength);
        bool ans = isNumber(firstCell);
        if (ans){      //int array
            int nextCell = 0;
            int i = 1;
            int len = 0;
            int startWord = i;
            int arraySize = j;
            int tmpArr [arraySize];
            while (i < toPrint.length()) {        //get the whole array
                len = 0;
                while(toPrint[i] != ']' && toPrint[i] != ',') {  //get a word
                    i++;
                    len++;
                }
                string value = toPrint.substr((unsigned long)startWord,(unsigned long)len);
                int intVal = stringToInt(value);
                tmpArr[nextCell] = intVal;
                nextCell++;
                i = i + 1;
                startWord = i;
            }
            IntArray newIntArray = IntArray(tmpArr,arraySize,line,"intArray");
            cout << newIntArray;
        }else
        if (firstCell == "true" || firstCell == "false"){      //bool array
            int nextCell = 0;
            int i = 1;
            int len = 0;
            int startWord = i;
            int arraySize = j;
            bool tmpArr [arraySize];
            while (i < toPrint.length()) {        //get the whole array
                len = 0;
                while(toPrint[i] != ']' && toPrint[i] != ',') {  //get a word
                    i++;
                    len++;
                }
                string value = toPrint.substr((unsigned long)startWord,(unsigned long)len);
                bool boolVal = value == "true";
                tmpArr[nextCell] = boolVal;
                nextCell++;
                i = i + 1;
                startWord = i;
            }
            BoolArray newBoolArray = BoolArray(tmpArr,arraySize,line,"boolArray");
            cout << newBoolArray;
        } else
        if (toPrint[1] == '"'){       //string array
            int nextCell = 0;
            int i = 1;
            int len = 0;
            int startWord = i;
            int arraySize = j;
            string tmpArr [arraySize];
            while (i < toPrint.length()) {        //get the whole array
                len = 0;
                while(toPrint[i] != ']' && toPrint[i] != ',') {  //get a word
                    i++;
                    len++;
                }
                string value = toPrint.substr((unsigned long)startWord,(unsigned long)len);
                string stringVal = value.substr(1,value.length()-2);
                tmpArr[nextCell] = stringVal;
                nextCell++;
                i = i + 1;
                startWord = i;
            }
            StringArray newStringArray = StringArray(tmpArr,arraySize,line,"stringArray");
            cout << newStringArray;
        }
    } else {
        bool isArr = false;
        string arrName;
        string arrLoc;
        if (toPrint.find('[') != std::string::npos) {
            if(toPrint[0] == '[') {
                isArr = true;
            }
            else {
                int iter1 = 0;
                while (toPrint[iter1] != '[')
                    iter1++;
                arrName = toPrint.substr(0, (unsigned long) iter1);
                iter1++;
                int iter2 = 0;
                int counter = iter1;
                while (toPrint[counter] != ']') {
                    iter2++;
                    counter++;
                }
                arrLoc = toPrint.substr((unsigned long) iter1, (unsigned long) iter2);
                toPrint = arrName;
                isArr = true;
                if (!(isNumber(arrLoc))) {
                    if (initializedVariables.doesNameExists(arrLoc)) {
                        if (initializedVariables.getVariable(arrLoc).getType() == "int") {
                            int value = ((IntVariable &) initializedVariables.getVariable(arrLoc)).getVal();
                            intToString(value, arrLoc);
                        } else {
                            throw BadVariableTypeException(lineNum);
                        }
                    } else {
                        if(arrLoc.find('+') != std::string::npos || arrLoc.find('=') != std::string::npos) {
                            Variable &ans = normalParse(arrLoc, lineNum, 0, initializedVariables);
                            if(ans.getType() != "int")
                                throw BadVariableTypeException(lineNum);
                            int value = ((IntVariable &)ans).getVal();
                            delete(&ans);
                            intToString(value, arrLoc);
                        } else
                            throw VariableNotFoundException(lineNum);
                    }
                }
            }
        }
        if(initializedVariables.doesNameExists(toPrint)) {
            string type = initializedVariables.getVariable(toPrint).getType();
            if(isArr) {
                if(type == "intArray") {
                    IntArray &iA = (IntArray&) initializedVariables.getVariable(toPrint);
                    int arrLocInt = stringToInt(arrLoc);
                    int arrSize = iA.getSize();
                    if (arrLocInt > arrSize - 1)
                        throw ArrayOutOfBoundException(lineNum);
                    IntVariable &i = (IntVariable &) *(iA.getIntElement(arrLocInt));
                    cout << i << endl;
                }
                else
                if(type == "boolArray") {
                    BoolArray &bA = (BoolArray&)initializedVariables.getVariable(toPrint);
                    int arrLocInt = stringToInt(arrLoc);
                    int arrSize = bA.getSize();
                    if (arrLocInt > arrSize - 1)
                        throw ArrayOutOfBoundException(lineNum);
                    BoolVariable &b = (BoolVariable &) *(bA.getBoolElement(arrLocInt));
                    cout << b << endl;
                }
                else
                if(type == "stringArray") {
                    StringArray &sA = (StringArray&)initializedVariables.getVariable(toPrint);
                    int arrLocInt = stringToInt(arrLoc);
                    int arrSize = sA.getSize();
                    if (arrLocInt > arrSize - 1)
                        throw ArrayOutOfBoundException(lineNum);
                    StringVariable &s = (StringVariable &) *(sA.getStringElement(arrLocInt));
                    cout << s << endl;
                }
            }
            else {
                if (type == "int") {
                    IntVariable &i = (IntVariable&)initializedVariables.getVariable(toPrint);
                    cout << i << endl;
                }
                else
                if (type == "bool") {
                    BoolVariable &b = (BoolVariable&)initializedVariables.getVariable(toPrint);
                    cout << b << endl;
                }
                else
                if (type == "string") {
                    StringVariable &s = (StringVariable&)initializedVariables.getVariable(toPrint);
                    cout << s << endl;
                }
                else
                if(type == "intArray") {
                    IntArray &iA = (IntArray&)initializedVariables.getVariable(toPrint);
                    cout << iA;
                }
                else
                if(type == "boolArray") {
                    BoolArray &bA = (BoolArray&)initializedVariables.getVariable(toPrint);
                    cout << bA;
                }
                else
                if(type == "stringArray") {
                    StringArray &sA = (StringArray&)initializedVariables.getVariable(toPrint);
                    cout << sA;
                }
            }
        }
        else {
            throw VariableNotFoundException(lineNum);
        }
    }


}

/*
 * checks that the if statment is legal - contains a legal expression
 * and correct suing of initialized Variables.
 * if the statment is illegal an error will be thrown.
 * line - the given line of an if statment.
 * lineNum - the current line number.
 * initializedVariables - the current initialized Variables.
 */
void ifParse(const string &line,int lineNum,InitializedVariables &initializedVariables)
{
    int statementPos = (int)line.find("){");
    statementPos = statementPos + 2;
    bool elseExists = false;
    bool commentExists = false;
    int commentLoc;
    if (line.find("}else{") != std::string::npos) {
        if(line.find("}#") != std::string::npos) {
            commentLoc = (int) line.find("}#");
            commentLoc++;
            commentExists = true;
            elseExists = true;
        }
        else
            elseExists = true;
    }
    else
    if (line.find("}#") != std::string::npos) {
        commentLoc = (int) line.find("}#");
        commentLoc++;
        commentExists = true;
    }
    string condition = line.substr(3, line.length() - 5 - (line.length() - ((unsigned long)statementPos)));
    string type;
    bool value;
    Variable *tempAns;
    bool isRightFromOp = (condition.find("]+") != std::string::npos) || (condition.find("]=") != std::string::npos);
    bool isComma = condition.find(',') != std::string::npos;
    int nameLoc = (int)condition.find('[');
    int backCount = nameLoc;
    if(nameLoc != std::string::npos && !isRightFromOp && !isComma)
        while(backCount > 0 && condition[backCount] != '+' && condition[backCount] != '=') {
            backCount--;
        }
    bool isLeftFromOp = false;
    if(nameLoc != std::string::npos && !isRightFromOp && !isComma)
        isLeftFromOp = (condition[backCount] == '+' || condition[backCount] == '=');
    if(((condition.find('+') != std::string::npos || condition.find('=') != std::string::npos) && !(condition.find('[') != std::string::npos)) || ((condition.find('[') != std::string::npos) && isRightFromOp) || ((condition.find('[') != std::string::npos) && isLeftFromOp)) {
        tempAns = &normalParse(condition, lineNum, 0, initializedVariables);
        type = tempAns->getType();
        if(tempAns->getType() == "bool")
            value = ((BoolVariable*)tempAns)->getValue();
        delete(tempAns);
    }
    else {
        if(condition == "true" || condition == "false") {
            value = condition == "true";
            type = "bool";
        }
        else {
            bool isArr = false;
            string arrName;
            string arrLoc;
            if (condition.find('[') != std::string::npos) {
                if(condition[0] == '[') {
                    isArr = true;
                }
                else {
                    int iter1 = 0;
                    while (condition[iter1] != '[')
                        iter1++;
                    arrName = condition.substr(0, (unsigned long) iter1);
                    iter1++;
                    int iter2 = 0;
                    int counter = iter1;
                    while (condition[counter] != ']') {
                        iter2++;
                        counter++;
                    }
                    arrLoc = condition.substr((unsigned long) iter1, (unsigned long) iter2);
                    condition = arrName;
                    isArr = true;
                    if (!(isNumber(arrLoc))) {
                        if (initializedVariables.doesNameExists(arrLoc)) {
                            if (initializedVariables.getVariable(arrLoc).getType() == "int") {
                                int value = ((IntVariable &) initializedVariables.getVariable(arrLoc)).getVal();
                                intToString(value, arrLoc);
                            } else {
                                throw BadVariableTypeException(lineNum);
                            }
                        } else {
                            if(arrLoc.find('+') != std::string::npos || arrLoc.find('=') != std::string::npos) {
                                Variable &ans = normalParse(arrLoc, lineNum, 0, initializedVariables);
                                if(ans.getType() != "int")
                                    throw BadVariableTypeException(lineNum);
                                int value = ((IntVariable &)ans).getVal();
                                delete(&ans);
                                intToString(value, arrLoc);
                            } else
                                throw VariableNotFoundException(lineNum);
                        }
                    }
                }
            }
            if (initializedVariables.doesNameExists(condition)) {
                string type1 = initializedVariables.getVariable(condition).getType();
                if (isArr && type1 == "boolArray") {
                    BoolArray *bA = &(BoolArray &) initializedVariables.getVariable(condition);
                    int arrLocInt = stringToInt(arrLoc);
                    int arrSize = bA->getSize();
                    if (arrLocInt > arrSize - 1)
                        throw ArrayOutOfBoundException(lineNum);
                    tempAns = (Variable *) bA->getBoolElement(arrLocInt);
                    type = tempAns->getType();
                    if (type == "bool")
                        value = ((BoolVariable *) tempAns)->getValue();
                } else {
                    tempAns = &initializedVariables.getVariable(condition);
                    type = tempAns->getType();
                    if (type == "bool")
                        value = ((BoolVariable *) tempAns)->getValue();
                }
            } else
                throw BadVariableTypeException(lineNum);
        }
    }
    if(type == "bool") {
        if(value) {
            if(elseExists) {
                int elsePos = (int)line.find("}else{");
                string ifStatement = line.substr((unsigned long)statementPos,line.length() - (unsigned long)statementPos - (line.length() - (unsigned long)elsePos));
                if (ifStatement.find("print(") != std::string::npos) {
                    printParse(ifStatement, lineNum, initializedVariables);
                } else {
                    Variable *unused = &normalParse(ifStatement, lineNum, 0, initializedVariables);
                    delete(unused);
                }
            }
            else {
                string ifStatement;
                if(commentExists)
                    ifStatement = line.substr((unsigned long)statementPos,line.length() - (unsigned long)statementPos - 1 - (line.length() - (commentLoc - 1)));
                else
                    ifStatement = line.substr((unsigned long)statementPos,line.length() - (unsigned long)statementPos - 1);
                if (ifStatement.find("print(") != std::string::npos) {
                    printParse(ifStatement, lineNum, initializedVariables);
                } else {
                    Variable *unused = &normalParse(ifStatement, lineNum, 0, initializedVariables);
                    delete(unused);
                }
            }
        }
        else {
            if(elseExists) {
                int elseStatementLoc = (int)line.find("}else{");
                elseStatementLoc = elseStatementLoc + 6;
                string elseStatement;
                if(commentExists)
                    elseStatement = line.substr((unsigned long)elseStatementLoc,line.length() - elseStatementLoc - 1 - (line.length() - commentLoc));
                else
                    elseStatement = line.substr((unsigned long)elseStatementLoc,line.length() - elseStatementLoc - 1);
                if (elseStatement.find("print(") != std::string::npos) {
                    printParse(elseStatement, lineNum, initializedVariables);
                } else {
                    Variable *unused = &normalParse(elseStatement, lineNum, 0, initializedVariables);
                    delete(unused);
                }
            }
        }
    }
    else
        throw BadVariableTypeException(lineNum);
}

int main(int arg, char *args[]) {
    InitializedVariables* initializedVariables = new InitializedVariables();
    string line;
    int lineNum = 0;
    ifstream myfile (args[1]);
    //ifstream myfile ("scriptfile.txt");
    try {
        for (string line; getline(myfile, line);) {
            lineNum++;
            if (line[0] == '#')
                continue;
            if (line.find("if(") != std::string::npos) {
                ifParse(line, lineNum, *initializedVariables);
            } else if (line.find("print(") != std::string::npos) {
                printParse(line, lineNum, *initializedVariables);
            } else {
                Variable* unused = &normalParse(line, lineNum, 0, *initializedVariables);
                delete(unused);
            }
        }
        initializedVariables->deleteAll();
        delete(initializedVariables);
    }
    catch (Exception &e){
        e.Handle();
        initializedVariables->deleteAll();
        delete(initializedVariables);
    }

    return 0;
}