#include <iostream>
#include <vector>

class StudentGrade {
public:
    int grade{ 0 };

    StudentGrade(int grade) : grade(grade)
    {
        if (grade < 0 || grade > 100)
        {
            throw std::out_of_range("Invalid student grade.");
        }
    }

    ~StudentGrade()
    {
        std::cout << "Studend destructor called.\n";
    }
};

void doSomethingFaulty() // throw(std::exception) // trhow Keyword is deprecated since C++11 
{
    try
    {
        std::exception myException("Something very faulty occured.");
        std::cout << "&myException = " << &myException << std::endl;
        throw myException;
    } 
    catch (const std::exception& exc)
    {
        std::cout << "at first catch &myException = " << &exc << std::endl;
        throw; // Rethrow the incoming exc object
    }
}

template<class Func>
struct StructWithoutBody;

template<class A0>
struct StructWithoutBody<void (*)(A0)>
{
    int Arity{ 1 };
};

template<class A0, class A1>
struct StructWithoutBody<void (*)(A0, A1)>
{
    int Arity{ 2 };
};

//template <typename Function>
//struct MabFunctionTraits;
//
//template <typename Obj, typename R>
//struct MabFunctionTraits<5>
//{
//    Obj a;
//    R b;
//};

template<int a>
void doSomething()
{
    std::cout << a << std::endl;
}

void receivesOneArg(int a) {
    std::cout << a << std::endl;
}

int main()
{
    {
        std::vector<int> vec;

        try
        {
            vec.at(2);
        }
        catch (const std::out_of_range ex)
        {
            std::cout << ex.what() << std::endl;
        }
        catch (const std::exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    // Handle any exception
    {
        try
        {
            // throw 42; // You were able to throw an exception of any type
            throw "Other error of type const char*";
        }
        /*catch (const char* err)
        {
            std::cout << "Handled: " << err << std::endl;
        }*/
        catch (...)
        {
            std::cout << "Handled unkown type exception" << std::endl;
        }
    }

    // Check if exception is the same
    {
        try
        {
            doSomethingFaulty();
        }
        catch (const std::exception& ex)
        {
            std::cout << "at second catch &myException = " << &ex << std::endl;
        }
    }

    // Exception on constructor
    {
        try
        {
            StudentGrade obj{ -5 };
        }
        catch (const std::exception& ex)
        {
            std::cout << "Handled exception: " << ex.what() << std::endl;
        }
    }

    {
        StructWithoutBody<void (*)(int)> a;
        std::cout << "Arity: " << a.Arity << std::endl;

        StructWithoutBody<void (*)(int, float)> b;
        std::cout << "Arity: " << b.Arity << std::endl;
    }
}