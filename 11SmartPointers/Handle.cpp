#include "Handle.h"
#include "Body.h"

Handle::Handle() : implementation(new Body)
{
	std::cout << "Created the body!!\n";
}

Handle::Handle(Handle&&) noexcept = default;

Handle& Handle::operator=(Handle&&) noexcept = default;

Handle::~Handle() = default;

void Handle::sayHello()
{
	implementation->sayHello();
}
