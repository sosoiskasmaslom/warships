
#include <iostream>
#include "warship.h"
using namespace std;

int main(int argc, char** argv)
{
    unsigned port = (argc-1) ? (unsigned)stoul(argv[1]) : 8080;
    cout << "Port is " << port << endl;
    Warship nemat{string("127.0.0.1"), port};
    try
    { nemat.game(); }
    catch (const EndGame& e) 
    { std::cerr << e.what() << '\n'; }
}