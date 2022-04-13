#include <iostream>
#include <string>

// Sequential
#include <array>
#include <forward_list>
#include <list>
#include <deque>
#include <queue>

// Associative
#include <set>
#include <map>

class fridge
{
public:
    fridge(int a, bool b, bool c) : a(a), b(b), c(c) {}

private:
    int a, b, c;
};

int main()
{
    {
        std::array<int, 3u> p = std::array{ 5, 1, 2 }; // Arrays have store in the stack (unlike vectors)
        std::array<int, 3u>* q = new std::array{ 5, 1, 2 }; // However you can instance an array in the heap
        crbegin(p);
    }

    {
        // Forward linked list
        // No reverse iterator
        std::forward_list p{ 1, 2, 3, 4 };
        for (auto el : p)
        {
            std::cout << el << ", ";
        }
        std::cout << std::endl;
    }

    {
        // Double linked list
        // Reverse and forward iterator
        std::list p{ 1, 2, 3, 4 };
        for (auto el : p)
        {
            std::cout << el << ", ";
        }
        std::cout << std::endl;
    }

    {
        // Double ended queue
        // Efficiente insertion in front of queue (unlike vector)
        std::deque p{ 1, 2, 3, 4 };
        for (auto el : p)
        {
            std::cout << el << ", ";
        }
        std::cout << std::endl;
    }

    {
        using namespace std::literals;
        std::map<std::string, int> p{
            { "romualdo"s, 1000000 },
            { "jose"s, 1000000 }
        };

        for (auto el : p)
        {
            std::cout << "(" << el.first << ", " << el.second << ") | ";
        }
        std::cout << std::endl;

        // Structured binding
        for (auto [name, usdSalary] : p)
        {
            std::cout << "(" << name << ", " << usdSalary << ") | ";
        }
        std::cout << std::endl;
    }

    {
        std::queue<int, std::vector<int>> myQueue;
        myQueue.push(1);
        myQueue.push(2);
        myQueue.push(3);
        myQueue.push(4);

        // Adaptor don't have iterators
        /*for (auto el : myQueue)
        {
            std::cout << el << ","
        }*/

        std::cout << std::endl;
    }

    {
        std::vector<fridge> myVec;
        
        fridge temp = fridge(1, true, true);
        myVec.push_back(temp); // using push_back(const _Ty& _Val)

        myVec.push_back(fridge(2, true, false)); // using push_back(_Ty&& _Val)

        // Used to avoid copying
        myVec.emplace(cbegin(myVec), 3, false, false); // Pass constructor params
    }
}