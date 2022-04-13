#pragma once
#include <memory>
// https://docs.microsoft.com/en-us/cpp/cpp/pimpl-for-compile-time-encapsulation-modern-cpp?view=msvc-170
// Implementation of the handle-body pattern using the 'pointer to implementation' idiom a.k.a: pimpl idiom, Cheshire Cat idiom or Compiler Firewall idiom

// The pimpl idiom is a modern C++ technique to hide implementation, to minimize coupling, and to
// separate interfaces.Pimpl is short for "pointer to implementation." You may already be familiar
// with the concept but know it by other names like Cheshire Cat or Compiler Firewall idiom.
// 
// Why use pimpl ?
// Here's how the pimpl idiom can improve the software development lifecycle:
// - Minimization of compilation dependencies.
// - Separation of interface and implementation.
// - Portability.
// The pimpl idiom avoids rebuild cascades and brittle object layouts. It's well suited for (transitively) popular types.

// Forward declare the Body class
class Body;

class Handle
{
private:
	// Uses the RAII idiom to manage the pointer
	std::unique_ptr<Body> implementation{ nullptr };

public:
	Handle();
	// Copy operators won't be synthesized by compiler because the unique pointer member is not copyable
	
	// Move constructor and operator won't be synthesized by compiler when the destructor is defined
	// So let's declare them explicitely
	Handle(Handle&&) noexcept;
	Handle& operator=(Handle&&) noexcept;
	~Handle();
	void sayHello();
};

