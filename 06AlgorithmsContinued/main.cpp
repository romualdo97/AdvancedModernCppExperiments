#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

struct MyStruct {
    char data[14];
};

int main()
{
    // Null pointer experiment
    {
        // int is 4bytes
        int* zeroPtr = 0;
        int* nextAfterZero = ++zeroPtr;
        std::cout << nextAfterZero << std::endl;
    }
    {
        // double is 8bytes
        double* zeroPtr = 0;
        double* nextAfterZero = ++zeroPtr;
        std::cout << nextAfterZero << std::endl;
    }
    {
        MyStruct* zeroPtr = 0;
        MyStruct* nextAfterZero = ++zeroPtr;
        std::cout << nextAfterZero << std::endl;
    }

    // Algorithm first of
    {
        std::vector<int> myNumbers{ 1, 2, 3, 4, 5, 5, 6, 7, 8 };
        std::vector options{ 5, 6, 7 }; // C++17 can omit the type parameter
        auto resultIterator = std::find_first_of(cbegin(myNumbers), cend(myNumbers), cbegin(options), cend(options));
        if (resultIterator != cend(myNumbers)) {
            std::cout << *resultIterator << " at index " << distance(cbegin(myNumbers), resultIterator) << std::endl;
        }
    }

    // Adjacent find algorithm (Find to neghbouring algorithms with the same value)
    {
        std::vector<int> myNumbers{ 1, 2, 3, 4, 5, 6, 7, 7, 8 };
        auto firstMatch = std::adjacent_find(cbegin(myNumbers), cend(myNumbers));
        std::cout << *firstMatch << " at index " << distance(cbegin(myNumbers), firstMatch) << std::endl;
    }

    // Binary search & include algorithms
    {
        std::vector myNums{ 5, 6, 8, 4, 2, 3, 1, 5, 8, 9 };
        std::sort(begin(myNums), end(myNums));
        if (std::binary_search(cbegin(myNums), cend(myNums), 8))
        {
            std::cout << "8 exists in the list\n";
        }

        std::vector requiredNums{ 8, 2, 3 };
        std::sort(begin(requiredNums), end(requiredNums));
        if (std::includes(cbegin(myNums), cend(myNums), cbegin(requiredNums), cend(requiredNums)))
        {
            std::cout << "8, 2, 3 exists in the list\n";
        }
    }

    {
        std::vector originalList{ 1, 2, 3 };
        std::vector newList = originalList;
        newList.push_back(4);

    }

    {
        int myNums[4]{ 1, 2, 3, 4 };
        int* begin = myNums;

        //*begin++ = 5; // int* curr = begin++; *curr = 5;
        //*begin = 7; // *begin = 7

        int* curr = begin++; *curr = 5;
        *begin = 7;

        std::cout << myNums[0] << std::endl;
    }

    {
        std::vector myNums{ 1, 2, 3, 4 };

        do {
            for (auto num : myNums) {
                std::cout << num << ",";
            }
            std::cout << std::endl;
        } while (std::next_permutation(begin(myNums), end(myNums)));
    }

    {
        // std::random_device rd; // Takes entropy device data... if possible, otherwise is a pseaudo generator, this is usually passed as the seed ot the random engine
        std::default_random_engine eng; // functor
        std::uniform_int_distribution idist(0, 10); // distribution
        for (size_t i = 0; i < 5; i++)
        {
            std::cout << idist(eng) << ", "; // Call functor to get next random number
        }
        
        std::cout << std::endl;
    }

    {
        std::vector myNums{ 1, 2, 3, 4, 5, 6 };
        std::shuffle(begin(myNums), end(myNums), std::mt19937{});
        for (auto num : myNums) {
            std::cout << num << ", ";
        }
        std::cout << std::endl;
    }
    // search_n
    // search
    // mismatch
    // "some of" algorithms (receives predicates)
    //      - all_of
    //      - any_of
    //      - none_of
    // binary search
}
