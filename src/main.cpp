#include <iostream>
#include "warship.h"
using namespace std;

int main(int argc, char** argv)
{
    string name;
    cout << "Enter your name: ";
    getline(cin, name);

    string host_;
    cout << "Enter your invite code: ";
    getline(cin, host_);
    host_ = (host_.empty()) ? "127.0.0.1" : host_;

    unsigned port = (argc-1) ? (unsigned)stoul(argv[1]) : 5555;
    cout << "Port is " << port << endl;
    Warship nemat{host_, port, name}; 

    try
    { nemat.game(); }
    catch (const EndGame& e) 
    { std::cerr << e.what() << " WIN!!" << '\n'; }
}