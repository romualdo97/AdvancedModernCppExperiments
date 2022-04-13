#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std::string_literals;

int get()
{
    return 2;
}

void func(const int& a)
{
    std::cout << "lvalue reference: " << a << std::endl;
}

void func(const int&& a)
{
    std::cout << "rvalue reference: " << a << std::endl;
}

class MyClass{};

// Forwarding references
template<class T>
void funcForwarded(T&& a) // The T&& has special meaning when using on templates this is called forwarding reference or universal reference()
{ // Check this article: https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
    std::cout << "funcForwarded called\n";
}

class Test
{
private:
    int i{ 0 };
    MyClass m;

public:
    Test() = default;

    // Copy constructor
    Test(const Test& other) : i(other.i), m(other.m) {
        std::cout << "Calling copy constructor" << std::endl;
    }

    // Move constructor
    Test(Test&& other) noexcept : i(other.i), m(std::move(other.m)) {
        std::cout << "Calling move constructor" << std::endl;
    }

    // Copy assignment operator
    Test& operator=(const Test& other)
    {
        std::cout << "Calling copy assignment operator" << std::endl;
        i = other.i;
        m = other.m;
        return *this;
    }

    // Move assignment operator
    Test& operator=(Test&& other) noexcept
    {
        std::cout << "Calling move assignment operator" << std::endl;
        i = other.i;
        m = std::move(other.m);
        return *this;
    }
};


// My custom String class that uses the RAII idiom
class MyString {
private:
    char* data{ nullptr };
    size_t size{ 0 };

public:
    MyString() = default;

    // Constructor
    MyString(const std::string& s) : size(s.size())
    {
        std::cout << "MyString: Initialized MyString\n";

        // Allocate the resource (heap memory for the data)
        data = new char[size];

        // Populate the data
        std::copy(cbegin(s), cend(s), data); // for (size_t i = 0; i < size; i++) data[i] = s[i];
    }

    // Copy constructor
    MyString(const MyString& other) : size(other.size)
    {
        std::cout << "MyString: Copy constructor\n";
        // Do a depth copy on the resource... not a shallow one
        delete[] data;
        data = new char[size];
        std::copy(other.data, other.data + other.size, data); // for (size_t i = 0; i < size; i++) data[i] = other.data[i];
    }

    // Move constructor
    MyString(MyString&& other) noexcept : size(other.size)
    {
        std::cout << "MyString: Move constructor\n";
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }

    // Copy Assign operator
    MyString& operator=(const MyString& other)
    {
        std::cout << "MyString: Copy assign operator\n";

        if (this == &other)
        {
            return *this;
        }

        // Do a depth copy on the resource... not a shallow one
        delete[] data;
        data = new char[other.size];

        size = other.size;
        std::copy(other.data, other.data + other.size, data); // for (size_t i = 0; i < size; i++) data[i] = other.data[i];

        return *this;
    }

    // Move assign operator
    MyString& operator=(MyString&& other) noexcept
    {
        std::cout << "MyString: Move assign operator\n";

        if (this == &other)
        {
            return *this;
        }
        delete[] data;
        size = other.size;
        data = other.data;
        other.size = 0;
        other.data = nullptr;
        return *this;
    }

    // Destructor
    ~MyString()
    {
        std::cout << "MyString: Calling destructor\n";
        delete[] data; // Release the heap memory
    }

    size_t length() {
        return size;
    }
};

// Template metaprogramming
template<int N>
struct CompileTimeRecursion { // Find the base 2 power at compile time
    enum {
        // Enum values are conexpr (which allos the compiler to execute code)
        val = 2 * CompileTimeRecursion<N - 1>::val
    };
};

template<>
struct CompileTimeRecursion<0> {
    enum { val = 1 };
};

// Move-only class
class MyMoveOnlyClass {
public:
    MyMoveOnlyClass() = default; // Let the compiler synthesize/generate the default constructor
    MyMoveOnlyClass(const MyMoveOnlyClass&) = delete; // Delete the copy constructor
    MyMoveOnlyClass operator=(const MyMoveOnlyClass&) = delete; // Delete the copy-assignment operator
    
    MyMoveOnlyClass(MyMoveOnlyClass&& other) noexcept : i(other.i), m(std::move(other.m)) // Define the move constructor
    {
        std::cout << "Calling move constructor" << std::endl;
    };

    MyMoveOnlyClass& operator=(MyMoveOnlyClass&& other) noexcept // Define the move-assignment operator
    {
        std::cout << "Calling move assignment operator" << std::endl;
        if (this == &other) {
            return *this;
        }

        i = other.i;
        m = std::move(other.m); // Force the move-assignament operator to be called
    }

private:
    int i{ 0 };
    MyClass m;
};

void g(MyClass& x) {
    std::cout << "MyClass& x\n";
}

void g(const MyClass& x) {
    std::cout << "const MyClass& x\n";
}

void g(MyClass&& x) {
    std::cout << "MyClass&& x\n";
}

template<class T>
void f(T&& x) {
    //g(x);
    //g(std::move(x));
    g(std::forward<T>(x)); // Forward inputo into g
}

class MyCoolType {
    std::string m_str;
public:
    MyCoolType(const std::string& str) : m_str(str)
    {
        std::cout << "const MyCoolType& o\n";
    }

    MyCoolType(std::string&& str) noexcept : m_str(std::move(str))
    {
        std::cout << "MyCoolType&& o\n";
    }
};

template<class T>
MyCoolType make_cool_type(T&& str) {
    return MyCoolType(std::forward<T>(str));
}

int main()
{
    //{
    //    func(get());

    //    // Cast lvalue to rvalue using static cast
    //    int a{ 0 };
    //    func(std::move(a)); // Move don't moves anything... only cast arguments using static_cast from lvalue to rvalue i.e.: static_cast<int&&>(a)        func(std::move(a)); // Move don't moves anything... only cast arguments using static_cast from lvalue to rvalue i.e.: static_cast<int&&>(a)
    //    func(static_cast<int&&>(a)); // Similar to std::move

    //    // Pass a as lvalue reference
    //    func(a);

    //    MyClass b;
    //    MyClass& bRef = b;
    //    funcForwarded(b); // Compiler instantiates void funcForwarded<MyClass&>(MyClass& a);
    //    funcForwarded(bRef); // Compiler instantiates void funcForwarded<MyClass&>(MyClass& a);
    //    funcForwarded(std::move(b)); // Compiler instantiates void funcForwarded<MyClass>(MyClass&& a);
    //}

    //{
    //    Test a;

    //    // Calling copy constructor
    //    Test b(a);

    //    // Calling copy assign
    //    a = b;

    //    // Calling move constructor
    //    Test c(std::move(a));

    //    // Calling move assignment
    //    Test d;
    //    d = std::move(c);

    //    // Calling move assignment
    //    Test e;
    //    e = Test{};
    //}

    //{
    //    MyString a{ "Hello world"s };
    //    MyString b{ a };
    //    MyString c{ "c"s };
    //    c = b;

    //    MyString d{ std::move(c) };
    //    MyString e{ "eee" };
    //    MyString f;
    //    f = std::move(e);
    //}

    //{
    //    std::cout << "Template metaprogramming ";
    //    std::cout << CompileTimeRecursion<4>::val;
    //}

    //{

    //    // Call copy-assignment [error]
    //    MyMoveOnlyClass obj1;
    //    //MyMoveOnlyClass obj2 = obj1; // Error trying to call copy constructor
    //    //obj2 = obj1; // Error trying to call copy-assignment operator

    //    // Call move-constructor by using temporary object
    //    MyMoveOnlyClass obj3 = MyMoveOnlyClass();

    //    // Call move constructor
    //    MyMoveOnlyClass obj4 = std::move(obj3);

    //    // Call move constructor
    //    MyMoveOnlyClass obj5{ std::move(obj4) };

    //    // Call move-assignment operator
    //    MyMoveOnlyClass obj6;
    //    obj6 = std::move(obj5);
    //}

    //// Lambda expression move capture (2nd most reqested feature of C++14)
    //{
    //    std::vector<std::string> myVec{ "hola", "mundo", "happy", "i'm", "text" };

    //    // Capture by reference
    //    [&myVec]() {
    //        std::cout << "Capturing by reference | size: " << myVec.size() << std::endl;
    //    }(); // Invoke the lambda
    //    std::cout << "After capturing by reference | size: " << myVec.size() << std::endl;

    //    // Capture by move
    //    [movedVec = std::move(myVec)]() {
    //        std::cout << "Capturing by move | size: " << movedVec.size() << std::endl;
    //    }();
    //    std::cout << "After capturing by move | size: " << myVec.size() << std::endl;
    //}

    {
        MyClass a;
        const MyClass& ca = a;
        f(a);
        f(ca);
        f(std::move(a));
    }

    {
        std::string str = "Hello world";
        make_cool_type(str);
        make_cool_type("Hello world");
    }

    {
        std::unique_ptr<int> aPtr{ new int{5} };
        std::make_unique<int>(7);
        std::cout << *aPtr << std::endl;
    }
}
