#include <iostream>
#include <vector>
#include "Test.h"

using namespace std;
using namespace std::literals;

// Equivalente
//typedef vector<int> IntVec; // Classic typedef
using InVec = vector<int>; // Modern way

class MyClass
{
public:
    int a;
};

void doSomething(int* passedPtr)
{
    cout << "address of passedPtr : " << &passedPtr << endl;
    cout << "*passedPtr somewhat like memory[passedPtr] : " << *passedPtr << endl;
    *passedPtr = 5;
    cout << "*passedPtr = 5 somewhat like memory[passedPtr] = 5 : " << *passedPtr << endl;
}

void doSomethingWithRef(int& passedRef)
{
    cout << "address of passedRef : " << &passedRef << endl;
    passedRef = 6;
    cout << "passedRef = 6 : " << &passedRef << endl;

    /*cout << "*passedPtr somewhat like memory[passedPtr] : " << passedPtr << endl;
    *passedPtr = 5;
    cout << "*passedPtr = 5 somewhat like memory[passedPtr] = 5 : " << *passedPtr << endl << endl;*/
}

void testValueSemantics(MyClass passedData) {
    passedData.a = 5;
    cout << "At testValueSemantics passedData.a = 5 " << endl;
}

void printVector(const std::vector<int>& vec)
{
    for (auto element : vec)
    {
        cout << element << ',';
    }
    cout << endl;
}

void BadlyWrittenPrint(char * str) {
    cout << str << endl;
}

template<class T>
T doSomethingGeneric(const T& a, const T& b)
{
    if (a > b) {
        return a;
    }

    return b;
}

int main()
{
    int a = 4;
    int* aPtr = &a;
    int& aAlias = a;
    cout << "address of a " << &a << endl << endl;
    cout << "address of aPtr " << &aPtr << endl;
    doSomething(aPtr);
    cout << "a " << a << endl << endl;
    doSomethingWithRef(a);
    cout << "a " << a << endl << endl;

    cout << "TESTING COPY SEMANTICS" << endl << endl;
    MyClass data{};
    cout << "data.a " << data.a << endl;
    testValueSemantics(data);
    cout << "data.a " << data.a << endl << endl;

    cout << "C++11 INITIALIZATION" << endl;
    int num{ 5 }; // int a = 5;
    string str{ "Hello world" }; // string str = "Hello world";
    vector<int> list{ 1, 2, 3, 4, 5 }; // vector with elements 1,2,3,4,5
    printVector(list);
    cout << "TESTING ASSIGNMENTS" << endl;
    Test test{ 2, "hola" };
    cout << "test.a " << test.a << ", test.str " << test.str << endl;
    Test otherTest{ 3, "bye" };
    cout << "otherTest.a " << otherTest.a << ", otherTest.str " << otherTest.str << endl;
    otherTest = test;
    cout << "otherTest.a " << otherTest.a << ", otherTest.str " << otherTest.str << endl;

    Test copy = test;

    string concatExample = "Hello world!!!"s + " I'm another string" + " another one";
    string rawStringLiteral = R"(C:\"Progam Files"\)";
    string rawStringLiteralEscapedParenthesis = R"x(C:\"Progam Files (x86)"\)x";

    cout << concatExample << endl;
    cout << rawStringLiteral << endl;
    cout << rawStringLiteralEscapedParenthesis << endl;

    char c = static_cast<char>(65);
    const char* demoStr = "Hello world";
    BadlyWrittenPrint(const_cast<char*>(demoStr));

    // Iterators
    string myDemoStr{ "Hello world!! using iterators" };
   /* string::iterator iterator = myDemoStr.begin();
    while (iterator != myDemoStr.end())
    {
        cout << *iterator << ',';
        ++iterator;
    }*/
    for (auto it = myDemoStr.begin(); it != myDemoStr.end(); ++it)
    {
        cout << *it << ',';
    }
    cout << endl;
    for (auto it = myDemoStr.cbegin(); it != myDemoStr.cend(); ++it)
    {
        cout << *it << ',';
    }
    cout << endl;
    for (auto it = myDemoStr.rbegin(); it != myDemoStr.rend(); ++it)
    {
        cout << *it << ',';
    }
    cout << endl;
    for (auto it = myDemoStr.crbegin(); it != myDemoStr.crend(); ++it)
    {
        cout << *it << ',';
    }
    cout << endl;
    for (auto it = begin(myDemoStr); it != end(myDemoStr); ++it)
    {
        cout << *it << ',';
    }
    cout << endl;

    // Range for loops
    for (auto el : myDemoStr)
    {
        cout << el << ',';
    }
    cout << endl;

    // Range for loops with modifications
    for (auto& el : myDemoStr)
    {
        el = 'q';
        cout << el << ',';
    }
    // Equivalent to
    /*for (auto it = begin(myDemoStr); it != end(myDemoStr); ++it)
    {
        char& el = *it;
        el = 'q';
        cout << el << ',';
    }*/
    cout << endl;

    // C++17 initializer in if statemant as in foor loops
    if (auto iter = begin(myDemoStr); iter != end(myDemoStr))
    {
        // Initializer in switch statement
        switch (int a = 0; a)
        {
        case 2:
            // C++17 attributes (not like Java attributes)
            [[fallthrogh]]; // Fallthrough to next case... not a mistake!!!
        case 1:
            [[fallthrough]]; // Fallthrough to next case... not a mistake!!!
        case 0:
            // Do something
            break;
        default:
            break;
        }
    }

    const string& p{ 6 };
    auto q = p;
    vector myVec{ 1,2,3 };
    doSomethingGeneric<int>(5, 3);
   
    // Create a type alias for the func pointer type
    //using funcSignature = void (*)(char*);
    //typedef void (*funcSignature)(char*);
    auto funcPointer = &BadlyWrittenPrint;
    (*funcPointer)(const_cast<char*>("hola"));
}