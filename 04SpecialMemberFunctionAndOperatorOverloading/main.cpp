#include <iostream>
#include <string>

using namespace std::literals;

// If we dont define special member functions the compiler will sinthesized them for us
// In modern C++ there are 5 special function... Traditionally they are defined only if there is no user version 
// With the move semantics they are synthesized based on other special memeber function 
// - constructor
// - copy constructor
// - assign copy operator
// - some new member functions in modern C++ for move operations
// - destructor

class refrigerator
{
    int temperature{ 0 };
public:
    // Default constructor
    refrigerator() : refrigerator(2) // C++11 added delegating constructors (allows one constructor to call another constructor
    {
        std::cout << "Default constructor\n";
    }

    // Super verbose constructor
    refrigerator(int temperature) : temperature(temperature) {
        std::cout << "Verbose constructor\n";
    }

    // Copy constructor
    refrigerator(const refrigerator& other) : temperature(other.temperature)
    {
        std::cout << "Copy constructor\n";
        // If we dont write this compiler generates a copy constructor automatically (called the sintethized copy constructor)
        // Usually that's good enough, this is required only when we need to override default behaviour
        // when we manage resources (heap memory, files, thread, database connection, etc...)
    }

    // Assign copy operator
    refrigerator& operator =(const refrigerator& other)
    {
        // Compiler also generates on of this... usually when managing resources
        // note using the assign copy modifies an already setup object, so if I change the value
        /// I may be generating a amemory leak... so before copying we need to deallocate or free the original resource
        temperature = other.temperature;
        std::cout << "Assign copy operator\n";
        return *this;
    }
};

// My custom String class that uses the RAII idiom
class MyString {
    char* data;
    size_t size;
public:
    // Constructor
    MyString(const std::string& s) : size(s.size())
    {
        std::cout << "Initialized MyString\n";

        // Allocate the resource (heap memory for the data)
        data = new char[size];

        // Populate the data
        for (size_t i = 0; i < size; i++)
        {
            data[i] = s[i];
        }
    }

    // Copy constructor
    MyString(const MyString& other) : size(other.size)
    {
        std::cout << "Copy constructor\n";
        // Do a depth copy on the resource... not a shallow one
        // delete[] data;

        data = new char[size];
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    // Assign copy operator
    MyString& operator=(const MyString& other)
    {
        if (this == &other)
        {
            return *this;
        }

        // Do a depth copy on the resource... not a shallow one
        size = other.size;
        delete[] data;

        data = new char[size];
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    // Destructor
    ~MyString()
    {
        std::cout << "Calling destructor\n";
        delete[] data; // Release the heap memory
    }

    size_t length() {
        return size;
    }
};

class MyClass {
public:
    MyClass()
    {
        std::cout << "Default constructor\n";
    }

    // Copy constructor
    MyClass(const MyClass& other)
    {
        std::cout << "Copy constructor\n";
    }
};

MyClass elision() {
    std::cout << "called?\n";
    return MyClass{};
    // Without elision should occur
    // - default constructor
    // - copy constructor (copy object to return space)
    // - copy constructor (copy object to var in main function)
    // Wit helision the compiler performs a return value optimization (invoking only the default constructor)
}

class CoolClass {
    float f{ 0 };
public:
    // Default constructor
    CoolClass() {}

    // A constructor with a single argument acts as an implicit conversion operator
    explicit CoolClass(float a) : f(a) // Explicit make sure we cant create a CoolClass from a literal float
    {

    }

    // Conversion operators
    operator int() const { // implicit conversion
        // Converts this class to int
        return 1;
    }

    explicit operator std::string() const { // explicit conversion
        return "cool class"s;
    }

    explicit operator bool() const { // Explicit conversion ignored inside if(HERE)
        return true;
    }
};

class SpecialClass {
    int a{ 0 };
    int b{ 1 };

public:
    // Modern C++ allows progammers to force the compiler to synthesize special memeber functions
    // This is done by puttin =default after the function parameter list
    SpecialClass() = default; // Allows compiler to synthesize default constructor
    SpecialClass(const SpecialClass& other) = default; // Synthesize the default copy constructor

    // In traditional C++: mo make class non-copyable set copy constructor and assign copy as private
    // In modern C++ we use the keyword '= delete' to achieve this
    SpecialClass& operator=(const SpecialClass& other) = delete;

    // The function call operator (make this a callbale object) functor
    /*bool operator() (int a, float b){}
    std::string operator() (float a, float b) {}*/

    friend std::ostream& operator<<(std::ostream&, const SpecialClass&);
};

std::ostream& operator<<(std::ostream& out, const SpecialClass& obj)
{
    out << "(" << obj.a << ", " << obj.b << ")";
    return out;
}

int main()
{
    refrigerator myRefrigerator;
    refrigerator refrigeratorCopy{ myRefrigerator };
    refrigerator anotherRefrigeratorCopy = myRefrigerator;
    anotherRefrigeratorCopy = refrigeratorCopy;

    std::cout << std::endl;
    MyString s("1"s);
    MyString str2("Two"s);
    MyString str{ str2 };
    s = str2;

    // Copy elision
    std::cout << std::endl;
    MyClass el = elision();

    // Testing conversion operators
    std::cout << std::endl;
    CoolClass p;
    if (p) {
        std::cout << "Explicit bool() ignored\n";
    }
    std::cout << p + 1 << std::endl;
    std::cout << std::string{ static_cast<std::string>(p) } << std::endl;

    // Using the constructor with single argument (implicitly converts float to CoolClass)
    // This may casue problem when passing literal type to method that expects a CoolClass parameter... it will perform a silent conversion instead of causing compile error
    //CoolClass q = 3.0f; // Invalid if explicit on especific constructor
    CoolClass q{ 3.0f };

    SpecialClass u;
    SpecialClass v;
    std::cout << std::endl;
    std::cout << u << std::endl;
    // u = v;
    std::cout << std::endl;
}