#include <algorithm>
#include <iostream>
#include <vector>

// Predicate
bool reverseSort(const int& a, const int& b)
{
    return a > b;
}

// Predicate but as a functor
class sortPredicate {
public:
    bool operator() (const int& a, const int& b)
    {
        return a > b;
    }
};

class greaterEqThanN {
    int n;
public:

    greaterEqThanN(int n) : n(n) {}

    bool operator() (const int& a)
    {
        return a >= n;
    }
};

bool is_string_equal(const std::string& lhs, const std::string& rhs)
{
    return std::equal(cbegin(lhs), cend(lhs), cbegin(rhs), cend(rhs),
        [](char lc, char rc) { return toupper(lc) == toupper(rc); }
    );
}

class Test {
public:
    void countdown() {
        // Can't capture this using [&this] or [=this]
        // but it will be captured also using [&] or [=]
        // but this will be always passed as a reference
        // C++17 introduced capture this by value (immutable copy) by using [*this]
        [this]() {

        }();
    }
};

// Experimenting with the move semantics
class MyHolder {
    char* data{ nullptr };
    size_t size{ 0 };
public:
    // Default constructor
    MyHolder() = default;

    // Initialize with size
    MyHolder(int size) : size(size), data(new char[size])
    { 
        std::cout << "Initialized MyHolder at " << this << std::endl;
    }

    // Copy constructor
    MyHolder(const MyHolder& other) : size(other.size), data(new char[other.size])
    {
        std::cout << "MyHolder copy constructor at " << this << std::endl;;
        std::copy(other.data, other.data + other.size, data);
    }

    // Assign copy operator
    MyHolder& operator=(const MyHolder& other)
    {
        std::cout << "MyHolder(" << this << ") assign copy operator " << &other << std::endl;

        // Check auto assign, i.e.: x = x
        if (this == &other)
        {
            return *this;
        }

        delete[] data;
        size = other.size;
        data = new char[size];
        std::copy(other.data, other.data + other.size, data);
        return *this;
    }

    // Move semantic constructor
    MyHolder(MyHolder&& other) noexcept : data(other.data), size(other.size)
    {
        std::cout << "MyHolder move constructor at " << this << std::endl;;
        other.data = nullptr;
        other.size = 0;
    }

    // Move semantic assign operator
    MyHolder& operator=(MyHolder&& other) noexcept
    {
        std::cout << "MyHolder(" << this << ") assign move operator " << &other << std::endl;

        // Check auto assign, i.e.: x = x
        if (this == &other)
        {
            return *this;
        }

        delete[] data;
        size = other.size;
        data = other.data;
        other.data = nullptr;
        other.size = 0;
        return *this;
    }

    // Destructor
    ~MyHolder()
    {
        std::cout << "Destroyed MyHolder at " << this << std::endl;
        delete[] data;
    }

    void print()
    {
        std::cout << "Printing MyHolder " << this << std::endl;
    }
};

MyHolder createMyHolder()
{
    return MyHolder{ 7 };
}

template<class Predicate>
void doSomething(Predicate func)
{
    func(5, 4, 3);
}

int globalNum{ 54 };

int main()
{
    std::vector<int> myVec{ 5,9,8,54,4,12,2,3,5,4,8,9 };
    
    auto backIterator = back_inserter(myVec);
    *backIterator = 7;

    // Not supported on vector nor string
    /*auto frontIterator = front_inserter(myVec);
    *frontIterator = 1;*/

    auto inserterIterator = inserter(myVec, next(begin(myVec)));
    *inserterIterator = 116;

    for (int el : myVec)
    {
        std::cout << el << ',';
    }
    std::cout << std::endl;

    //std::cout << "After sort\n";
    ////std::sort(begin(myVec), end(myVec), reverseSort);
    //std::sort(begin(myVec), end(myVec), sortPredicate());
    //for (int el : myVec)
    //{
    //    std::cout << el << ',';
    //}

    /*std::cout << "Greater eq than N\n";
    auto found = std::find_if(begin(myVec), end(myVec), greaterEqThanN(9));
    if (found != end(myVec))
    {
        std::cout << "Greater eq than N is " << *found << std::endl;
    }*/

    //std::cout << "Is odd using lambda\n";
    //auto found = std::find_if(begin(myVec), end(myVec),
    //    [](int el) -> bool // the '-> bool' is required in C++11 which cant deduce the return type if there are more than one expression
    //    {
    //        return el % 2 == 1;
    //    }
    //);
    //if (found != end(myVec))
    //{
    //    std::cout << "First odd is " << *found << std::endl;
    //}

   /* if (is_string_equal("HOla", "Hola"))
    {
        std::cout << "is equal\n";
    }*/

    // Capturing
    static int localStaticNum{ 5 };
    int localNum{ 3 };
    int& localNum_r{ localNum };
    []() {
        std::cout << globalNum << std::endl;
        std::cout << localStaticNum << std::endl;
        //std::cout << localNum << std::endl; // ERROR
        //std::cout << localNum_r << std::endl; // ERROR James Raynord says this should be supported but it has no support by any compilers he have
    };

    // This creates a functor with state, where state variables are const
    int x{ 7 };
    [localNum, x]() {
        std::cout << globalNum << std::endl;
        std::cout << localStaticNum << std::endl;
        std::cout << localNum << std::endl;
        std::cout << x << std::endl;
    };

    //// Trying to modify captured value (but it is modifying the copy not the original value)
    //int y{ 8 };
    //std::cout << y << std::endl;
    //auto func = [y]() mutable {
    //    ++y; // 'y': a by copy capture cannot be modified in a non - mutable lambda
    //};
    //func();
    //std::cout << y << std::endl;

    // Capturing by reference
    //int y{ 8 };
    //std::cout << y << std::endl;
    //auto func = [&y]() mutable {
    //    ++y; // 'y': a by copy capture cannot be modified in a non - mutable lambda
    //};
    //func();
    //std::cout << y << std::endl;

    // Capture all variables in scope by value [=] or all in scope by reference [&] (probably not a good idea)
    // [=, &x] will capture x by reference all the other by value (capturing all by reference allow you to modify all variables in the scope, similar to javascript)
    // [&x, &y, =] will capture x and y by reference all other vars by value
    std::cout << localNum << std::endl;
    auto func = [&]() {
        ++localNum;
    };
    func();
    std::cout << localNum << std::endl;

    auto print = [](int a, int b, int c) { std::cout << a << ',' << b << ',' << c << '\n'; };
    doSomething(print);

    std::cout << std::endl;

    // int& a = 5; // [ERROR] initial value of reference to non - const must be an lvalue
    const int& a = 5; // Solves prev error

    int&& b = 5; // r-value reference
    ++b; // Can modify the temporal value
    // The moving semantics allow 'stealing' data from right values
    // and is used to avoid unnecesary invocations to the copy constructor or assign copy constructor when certain temporal value has been created and for example passed as argument to a function

    std::cout << "Hello " << b << std::endl;
    MyHolder holder1{ 4 }; // constructor
    MyHolder holder2{ 2 }; // constructor
    MyHolder holder4 = holder2; // copy constructor
    holder2 = holder1; // assign copy operator

    // Call the move constructor
    MyHolder robber{ std::move(holder1) };

    // Call
    // Should test on gcc vs doesnt seems to allo how to disable the RVO
    auto recivedHolder = createMyHolder(); // Cant see this in action here because of the RVO return value optimization
    std::cout << "Hello " << b << std::endl;    
}