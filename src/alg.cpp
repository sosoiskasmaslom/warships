
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// перевод числа в строке в тип unsigned
inline unsigned sti(std::string line)
{ return (unsigned)std::stoul(line); }

// перевод unsigned в 16ую систему
inline std::string to_hex(unsigned n)
{
    std::stringstream ss;
    ss << std::hex << std::uppercase << n;
    return ss.str();
}

// режет строку на массив подстрок по заданному char
// по стандарту на символы
inline std::vector<std::string> split(std::string line, char spl='\0')
{
    unsigned k = 0;
    std::vector<std::string> list;
    for(unsigned i = 0; i<line.size(); ++i)
    {
        if (spl == '\0')
        {
            list.push_back(line.substr(i, 1)); 
            continue;
        }
        else if (line[i] == spl)
        { 
            list.push_back(line.substr(k, i-k)); 
            k = i+1;
        }
    }
    if (line[line.size()-1] != ' ' && spl != '\0')
    { list.push_back(line.substr(k, line.size()-k)); }

    return list;
}

inline std::vector<std::string> division(std::string line)
{
    return std::vector<std::string> 
    {
        line.substr(0, line.size()/2),
        line.substr(line.size()/2, line.size()/2),
    };
}