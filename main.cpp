
#include <iostream>
#include "warship.h"
using namespace std;

int main(int argc, char** argv)
{
    string name;
    cout << "Enter your name: ";
    getline(cin, name);

    unsigned port = (argc-1) ? (unsigned)stoul(argv[1]) : 8080;
    cout << "Port is " << port << endl;
    Warship nemat{string("127.0.0.1"), port, name}; 

    try
    { nemat.game(); }
    catch (const EndGame& e) 
    { std::cerr << e.what() << " WIN!!" << '\n'; }
}