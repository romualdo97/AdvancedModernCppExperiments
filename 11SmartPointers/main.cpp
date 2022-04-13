#include <iostream>
#include <memory>
#include "Handle.h"

class MyClass {
public:
	~MyClass() {
		std::cout << "released my class memory!!!\n";
	}
};

// Code from C networking API
struct destination { /* Code */ };
struct connection { /* Code */ };

// Function to open a connection
connection connect(destination dest) {
	std::cout << "Connecting...\n";
	connection conn;
	return conn;
}

// Function to close a connection
void disconnect(connection conn) {
	std::cout << "Disconecting...\n";
}
// end of code of C networking API

// C++ function to get connection data 
void getData(const destination& dest) {
	connection conn = connect(dest);

	// Create the custom deleter... (can't know the name that compiler will give to this lamba at this moment untils compiler instances the class functor, we use decltype to solve this)
	auto endConnection = [](connection* conn) {
		std::cout << "Disconnecting using C API\n";
		disconnect(*conn);
	};

	std::unique_ptr<connection, decltype(endConnection)> ptr(&conn, endConnection); // unique_ptr takes owenership
	std::cout << "Getting data...\n";
	// delete conn???
}

template<class T>
class shared : public std::shared_ptr<T> {
public:
	shared(T* data) : std::shared_ptr<T>(data) {
		std::cout << "Created shared at " << this << " | total shared refs is " << this->use_count() << " | sharing " << this->get() << std::endl;
	}

	shared(const shared& other) : std::shared_ptr<T>(other) {
		std::cout << "Created shared at " << this << " using copy constructor | total shared refs is " << this->use_count() << " | sharing " << this->get() << std::endl;
	}

	shared& operator=(const shared& other) {
		std::shared_ptr<T>::operator=(other);
		std::cout << "Shared object at " << this << " using copy-assignement operator | total shared refs is " << this->use_count() << " | sharing " << this->get() << std::endl;
		return *this;
	}

	~shared() {
		std::cout << "=========================\n";
		std::cout << "---- Destroyed shared ptr object at " << this << std::endl;
		std::cout << "---- Whose shared memory is at " << this->get() << std::endl;
		std::cout << "---- Counted references before destroy " << this->use_count() << std::endl << std::endl;

		// need to call the base destructor method from a derived class in c++?
		// https://stackoverflow.com/questions/5551441/need-to-call-the-base-destructor-method-from-a-derived-class-in-c
		// You should never invoke a base class destructor from a derived class destructor.
		// The reason is base class destructor will be then called automatically for the second time
		// 
		// base destructors are invoked automatically in this case, in much the same way that base constructors can be invoked automatically.
		// Note, though, that if you are using polymorphismand destroying through a base pointer, you should make sure that the destructor is virtual otherwise this will break.
		// std::shared_ptr<T>::~shared_ptr();
	}
};

struct B;

struct A {
	shared<B> someData = nullptr;

	A() {
		std::cout << "A object created at: " << this << std::endl;
	}

	~A() {
		std::cout << "Destroying A object that lives at " << this << std::endl;
	}
};

struct B {
	shared<A> someData = nullptr;
	
	B() {
		std::cout << "B object created at: " << this << std::endl;
	}

	~B() {
		std::cout << "Destroying B object that lives at " << this << std::endl;
	}
};


struct baseClass {
	~baseClass() {
		std::cout << "Base destructor called\n";
	}
};


struct derivedClass : public baseClass {
	~derivedClass() {
		std::cout << "Derived destructor called\n";
	}
};

int main()
{
	//// Creation
	//{
	//	std::unique_ptr<int> intPtr = std::unique_ptr<int>(new int{5});
	//	std::unique_ptr<MyClass> objPtr = std::unique_ptr<MyClass>(new MyClass);

	//	auto myNum = std::make_unique<int>();
	//	auto myObj = std::make_unique<MyClass>();
	//}

	//// Custom destructors
	//{
	//	destination dest;
	//	getData(dest);
	//}

	//{
	//	// Client code can't use the backend directly because is an incomplete type from its perspective
	//	//Body backend; // ERROR: Incomplete type is not allowed

	//	Handle front;
	//	front.sayHello();
	//}

	//// Testing shared ptrs
	//{
	//	shared<A> a{ new A };
	//	shared<A> aCopy = a;
	//	shared<A> aCopyAgain = aCopy;
	//	shared<A> aCopyAgainAgain = aCopyAgain;

	//	std::cout << "\n\nWILL START DESTROYING:\n\n";
	//}

	// Testing the cyclic references problem with shared pointers
	// https://stackoverflow.com/questions/22185896/what-is-the-cyclic-dependency-issue-with-shared-ptr
	{
		shared<A> a{ new A };
		std::cout << std::endl;
		shared<B> b{ new B };

		std::cout << std::endl;
		a->someData = b;
		b->someData = a;

		std::cout << "\n\nWILL START DESTROYING:\n\n";
	}

	// Experimenting with destructor of non virtual classes
	/*{
		derivedClass a;
	}*/
}
