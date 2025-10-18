
#include "tunnel.h"
using namespace std;

Tunnel::Tunnel(unsigned port_):
host(1)
{ 
    server.jackhost_act(port_); 
    server.wait_jack();
}

Tunnel::Tunnel(string host_, unsigned port_):
host(0)
{ 
    try 
    { client.jack_act(host_, port_); } 
    catch(const exception& e)
    { 
        host = 1;
        cout << "Its server now" << endl;
        server.jackhost_act(port_); 
        server.wait_jack();
    }
}

Tunnel::~Tunnel()
{
    client.~jack();
    server.~jackhost();
}

bool Tunnel::check()
{
    try
    { 
        if (!host) 
        {
            msg = "1";
            client << msg; 
            client >> msg;
            
            if (msg == "1") 
            return 1;
        }
        else 
        {
            server >> msg;
            if (msg == "1")
            {
                server << msg;
                return 1;
            }
        }
    }
    catch(const std::exception& e)
    { cerr << e.what() << endl; }

    return 0;
}

bool Tunnel::send(string input)
{ 
    try
    {
        if (host)
        { server << input; }
        else 
        { client << input; }
        return 1;
    }
    catch(const std::exception& e)
    { cerr << e.what() << endl; }

    return 0;
}

string Tunnel::recieve()
{
    try
    {
        if (host)
        { server >> msg; }
        else 
        { client >> msg; }
        return msg;
    }
    catch(const std::exception& e)
    { cerr << e.what() << endl; }

    return "";
}

bool Tunnel::get_host() const
{ return host; }