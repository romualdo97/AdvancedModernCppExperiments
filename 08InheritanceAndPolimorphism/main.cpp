#include <iostream>
#include <string>

using namespace std::literals;

class MyBase
{
public:
	MyBase(int a) : a(a)
	{
		std::cout << "My base constructor" << std::endl;
	}

	void DoSomething()
	{
		std::cout << "Doing something at base\n";
	}

	void Draw() const
	{
		std::cout << "Drawing Mybase\n";
	}

	virtual void DrawVirtual() const
	{
		std::cout << "Draw MyBase virtual\n";
	}

private :
	int a;
};

// Final was introduced at C++11
class MyDerived final : public MyBase
{
public:

	MyDerived(int b) : MyBase(5), b(b)
	{
		std::cout << "Derived constructor" << std::endl;
	}

	// Overloading hides inherited version
	void DoSomething(int n)
	{
		std::cout << "Doing something "s << n << " times cooler\n";
	}

	// Old solution (before C++11)
	//void DoSomething()
	//{
	//	MyBase::DoSomething(); // This may become a nightmare when the inheritance tree es complex and big
	//}

	// New solution (C++11)
	using MyBase::DoSomething;

	void DoSomethingNew()
	{
		std::cout << "Doing something innovative\n";
	}

	void Draw() const
	{
		std::cout << "Drawing MyDerived\n";
	}

	virtual void DrawVirtual() const override final // Override is used to let the compiler check for overriding erros (e.g.: the base class ahas a virtual function with same name and signature)
	{
		std::cout << "Draw MyDerived virtual\n";
	}

private:
	int b;
};

//class MyDeluxedDerived : public MyDerived
//{
//	virtual void DrawVirtual() const override // Override is used to let the compiler check for overriding erros (e.g.: the base class ahas a virtual function with same name and signature)
//	{
//		std::cout << "Draw MyDeluxedDerived virtual\n";
//	}
//};

int main()
{
	{
		// Instance of base class
		MyBase baseObj{ 1 }; // Static type is MyBase

		// Instance of derived class
		MyDerived derivedObj{ 5 }; // Static type is MyDerived
		derivedObj.DoSomething();
		derivedObj.DoSomething(4);

		// Pointers
		{
			// Pointer to base class, points to a derived object
			MyBase* ptr = &derivedObj;

			// Error: Pointer to derived class, points to a base object
			// MyDerived* ptr = &baseObj;
		}

		// References
		{
			// Reference of base class, references a derived object
			MyBase& ptr = derivedObj;

			// Error: Reference of derived class, references a base object
			// MyDerived& ptr = baseObj;
		}

		// Pointers
		{
			// Pointer to base class, points to a derived object
			MyBase* ptr = &derivedObj; // Static type is MyBase*, Dynamic type is MyDerived
			//ptr->DoSomething();
			////ptr->DoSomethingNew(); // Error: Not a memeber of MyBase
			//ptr->Draw(); // Calls the draw at base... not the one at derived object

			// C++ variables have two different kind of types
			// - Static type and Dynamic type
			// Static type is the type used on variable declaration
			// Dynamic type is the type of the variable in memory
			// For most objects this will be the same as the static type
			// However, it can be different for a pointer or reference to a base class

			// The dynamic type is only used for a pointer or reference to a base classs
			// Resolved at runtime calls the MyDerived::DrawVirtual()
			ptr->DrawVirtual(); // No need to dynamic cast because DrawVirtual was declared virtual...

			// Calls the draw at the MyDerived instead of the one a the MyBase class
			ptr->Draw();
			dynamic_cast<MyDerived*>(ptr)->Draw(); // Can't do this cast if the base class don't have virtual members because it woldn't be a 'polimorphic type'
		}
	}
}