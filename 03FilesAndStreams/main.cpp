#include <iostream>
#include <fstream>
#include <sstream>
#include "bitmap.h"

using namespace std;

// We use fixed integers to make sure we get the same result on all systems
// #pragma pack(push, 1) // Override the default memory alignment settings
struct point {
    char c;
    int32_t x;
    int32_t y;
};
// #pragma pack(pop)

int main()
{
    //ifstream iFile{ "text.txt" };
    //if (iFile)
    //{
    //    cout << "File opened!\n";
    //    string text;
    //    //while (iFile >> text) // Reads a word, separated by white spaces (tab, spaces, newlines)
    //    //{
    //    //    cout << text << '*';
    //    //}

    //    //while (getline(iFile, text)) // Read a line of text from the input stream and put into the text variable
    //    //{
    //    //    cout << text << "*";
    //    //}

    //    /*char buffer[10];
    //    iFile.read(buffer, 10);
    //    cout << "Did read " << iFile.gcount() << " bytes\n";
    //    cout << "Content: " << buffer << " bytes\n";*/


    //    iFile.close();
    //}
    //else
    //{
    //    cout << "File did not open!\n";
    //}
    //cout << "\n\n";

    /*ofstream oFile{ "text_out.txt" };
    if (oFile)
    {
        cout << "File opened!\n";
        string text{"hola mundo!!!"};
        oFile << text;
        oFile.close();
    }
    else
    {
        cout << "File did not open!\n";
    }
    cout << "\n\n";*/

    // Append mode
    // trun: Open in truncate mode
    // in: Open for input
    // out: Open the file in output mode also in truncate mode , even if trun is not specified
    // ate: Similar to append, but output can be written anywhere in the file
    /*ofstream oFile("important.txt", fstream::app);
    if (oFile)
    {
        oFile << "Adding data" << std::boolalpha;
        oFile.close();
    }*/

    // stream manipulators
    // - std::flush
    // - std::endl (new line and flush)
    // - boolalpha (true and false as a string "true" or "false")
    // - noboolalpha (disable the prev manipulator)
    // - setw (at iomanip)
    // - left (justify left setw)
    // 
    // Double point manipulators
    // - fixed (dont show scientific notation on deicmal numbers)
    // - uppercase (show 1.23E+08 instead of 1.23e+08)
    // - scientific (all decimals are shown in scientific notation)
    // - defaultfloat (reset all the float manipulator settings)
    // - setprecision(3) (how many digits)
    // the gcount memeber function will return the number of characters(bytes) that were actually received auto nread = iFile.gCount()

    // Using ostream to convert number to string (instead of using to_string)
    /*ostringstream ostream;
    ostream << 3.141516;
    ostream.str();*/

    // What happens if to fstreams open the same file
    //fstream file("text.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    //if (file.is_open()) {
    //    file << "File 1";
    //    //file.close();
    //}
    //fstream file2("text.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    //if (file2.is_open()) {
    //    file2 << "file 2";
    //}

    //// Output stream iterators (dereference and increment does nothing... only for consistency with iterator code)
    //ostream_iterator<int> ostreamIterator{ cout, "\n" };
    //*ostreamIterator = 4;
    //++ostreamIterator;
    //*ostreamIterator = 5;
    //++ostreamIterator;

    //// Input stream iterators
    //int result;
    //istream_iterator<int> istreamIterator{ cin };
    //result = *istreamIterator;
    //cout << "You entered " << result;

    // Write and read binary data
    point myPoint;
    myPoint.c = 'A';
    myPoint.x = 1;
    myPoint.y = 2;

    // Output to binary file
    ofstream oBinary{ "point.bin" };
    if (oBinary) {
        const char* address = reinterpret_cast<char*>(&myPoint);
        cout << "sizeof(myPoint) = " << sizeof(myPoint) << endl; // Note sizeof is 12 not 9, this is because of memory alignment and padding
        oBinary.write(address, sizeof(myPoint));
        oBinary.close();
    }

    // Read from binary file
    point readedPoint;
    ifstream iBinary{ "point.bin" };
    if (iBinary) {
        char* adress = reinterpret_cast<char*>(&readedPoint);
        cout << "sizeof(readedPoint) = " << sizeof(readedPoint) << endl;
        iBinary.read(adress, sizeof(readedPoint));
        cout << "Did read " << iBinary.tellg() << " bytes" << endl;
        cout << "readedPoint.c = " << readedPoint.c << endl;
        cout << "readedPoint.x = " << readedPoint.x << endl;
        cout << "readedPoint.y = " << readedPoint.y << endl;
        iBinary.close();
    }

    // Write a BMP file
    bitmap myImage{ "helloImage.bmp" };
    if (myImage) {
        myImage.clearColor(0x34, 0x98, 0xdb);
        myImage.drawRect(50, 50, 100, 100);
        myImage.drawRect(150, 150, 100, 100);
        myImage.drawRect(250, 250, 100, 100);
        myImage.write();
    }
}
