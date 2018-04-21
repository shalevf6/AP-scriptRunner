scriptRunner:main.o Variable.o ArrayVariable.o IntArray.o BoolArray.o StringArray.o InitializedVariables.o IntVariable.o BoolVariable.o StringVariable.o Exception.o
	g++ main.o Variable.o ArrayVariable.o IntArray.o BoolArray.o StringArray.o InitializedVariables.o IntVariable.o BoolVariable.o StringVariable.o Exception.o -o scriptRunner
Exception.o:Exception.h Exception.cpp
	g++ -c Exception.cpp
StringVariable.o:StringVariable.cpp StringVariable.h Variable.h
	g++ -c StringVariable.cpp
BoolVariable.o:BoolVariable.cpp BoolVariable.h Variable.h
	g++ -c BoolVariable.cpp
IntVariable.o:IntVariable.cpp IntVariable.h Variable.h
	g++ -c IntVariable.cpp
InitializedVariables.o:InitializedVariables.cpp InitializedVariables.h Variable.h
	g++ -c InitializedVariables.cpp
StringArray.o:StringArray.cpp StringArray.h ArrayVariable.h StringVariable.h
	g++ -c StringArray.cpp
BoolArray.o:BoolArray.cpp BoolArray.h ArrayVariable.h BoolVariable.h
	g++ -c BoolArray.cpp
IntArray.o:IntArray.cpp IntArray.h ArrayVariable.h IntVariable.h
	g++ -c IntArray.cpp
ArrayVariable.o:ArrayVariable.cpp ArrayVariable.h Variable.h
	g++ -c ArrayVariable.cpp
Variable.o:Variable.cpp Variable.h
	g++ -c Variable.cpp
main.o:main.cpp InitializedVariables.h IntArray.h BoolArray.h StringArray.h Exception.h
	g++ -c main.cpp
clean:
	rm -g *.o
