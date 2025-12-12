
#include "tunnel.h"
#include "alg.cpp"
using namespace std;

Tunnel::Tunnel()
{}

Tunnel::Tunnel(unsigned port_):
Tunnel("127.0.0.1", port_)
{}

Tunnel::Tunnel(string host_, unsigned port_):
host(0)
{ 
    try 
    { client.jack_act(invite_to_ip(host_), port_); } 
    catch(const exception& e)
    { 
        host = 1;
        cout << "Its server now" << endl;
        cout << get_invite() << endl;
        server.jackhost_act(port_); 
        server.wait_jack();
    }
}

void Tunnel::set(unsigned port_)
{ set("127.0.0.1", port_); }

void Tunnel::set(string host_, unsigned port_)
{ 
    this->~Tunnel();
    new (this) Tunnel(host_, port_);
}

bool Tunnel::check() // чмо
/* ты должна была бороться со злом, 
   а не примкнуть к нему */
{}

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

string Tunnel::get_ip() const
{ 
    if (host) 
    { return server.get_ip(); }
    else 
    { return "";}
}

string Tunnel::get_invite() const
{ 
    if (host)
    { return server.get_invite(); }
    else 
    { return "";}
}

string Tunnel::invite_to_ip(const string& data)
{
    if (host)
    { return ""; }
    else
    { return client.invite_to_ip(data); }
}


sTunnel::sTunnel()
{}

sTunnel::sTunnel(unsigned port_):
Tunnel("127.0.0.1", port_)
{}

sTunnel::sTunnel(string host_, unsigned port_):
Tunnel(host_, port_)
{}

sTunnel::~sTunnel()
{ bue(); }

string sTunnel::srecieve()
{
    msg = recieve();
    if (msg == "BUE")
    { throw EndGame((*this)[0]); }

    return msg;
}

string sTunnel::hello(string name)
{
    names[0] = name;
    if (host)
    { send(name); }
    names[1] = srecieve();
    if (!host)
    { send(name); }
    return names[1];
}

bool sTunnel::ready()
{ 
    if (host) 
    { 
        send("READY"); 
        return (srecieve() == "READY");
    } else {
        if (srecieve() == "READY")
        { return send("READY"); }
        else
        { return 0; }
    }
}

void sTunnel::bue()
{ send("BUE"); }

bool sTunnel::restart()
{
    send("RESTART");
    string res = srecieve();
    if (res != "ACCEPT" && res != "DECLINE")
    { throw runtime_error("sTunnel::restart() - unknown recieve()"); }

    return (res == "ACCEPT");
}

void sTunnel::accept()
{ send("ACCEPT"); }

void sTunnel::decline()
{ send("DECLINE"); }

bool sTunnel::start()
{
    if (host)
    { return send("START"); }
    else
    { return (srecieve() == "START"); }

    throw runtime_error("sTunnel::start() - smth wrong with host");
}

void sTunnel::shot(const Point& pos)
{ send(string("SHOT ")+pos.to_string()); }

Point sTunnel::shot()
{
    vector<string> ss = split(srecieve(), ' ');
    if (ss[0] != "SHOT")
    { throw runtime_error("sTunnel::shot() - its not SHOT"); }

    return Point(ss[1]);
}

void sTunnel::result(const Point& pos, unsigned res, bool s)
{ send(string("RESULT ")+pos.to_string()+string(" ")+to_string(res)+string(" ")+to_string(s)); }

bool sTunnel::result()
{
    vector<string> ss = split(srecieve(), ' ');

    if (ss[0] != "RESULT")
    { throw runtime_error("sTunnel::result() - its not RESULT"); }

    if (!sti(ss[3]))
    { throw EndGame((*this)[0]); }

    step = ss[1];
    return sti(ss[2]);
}

string sTunnel::get_shot() const
{ return step; }

string sTunnel::get_name() const
{ return (*this)[1]; }

string sTunnel::operator[](bool s) const
{ 
    if (s)
    { return names[1]; }
    else 
    { return names[0]; }
}