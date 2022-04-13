#pragma once
#include <string>
#include <iostream>

using namespace std;

class Test
{
public:
	int a;
	string str;

	// Constructor
	Test(int a, const string& str) : a(a), str(str)
	{
		cout << "Initialized\n";
	}

	// Copy constructor
	Test(const Test& other) : a(other.a), str(other.str) {
		cout << "Copied\n";
	}

	// Assign copy operator
	Test& operator=(const Test& other) {
		cout << "Assign copied\n";
		a = other.a;
		str = other.str;
		return *this;
	}

	// Destroy
	~Test()
	{
		cout << "Destroyed\n";
	}
};