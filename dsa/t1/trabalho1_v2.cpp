#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

bool
exp(std::map<char, std::string> &m, std::stringstream &s)
{
    int counter{};
    int size = s.str().size();
    std::string aux = s.str();
    s.str("");

    for(int i{}; i < size; i++)
    { 
        if(m.count(aux[i]) != 0)
        {
            s << m.at(aux[i]);
            counter++;
            continue;
        }
        s << aux[i];
    }
    return (counter == 0) ? false : true;
}

int
main()
{
    std::ifstream file("casos_11/t11_02.txt");
    std::map<char, std::string> keymap;

    std::stringstream s;

    std::string line;

    // Read input file
    int size;
    while(getline(file, line)) 
    {
        size = line.size();
        if(size > 2)
        {
            keymap.emplace(line[0], line.substr(2, size - 2));
        }
    }

    file.close();
    
    // Find top letter
    char top = keymap.begin()->first;
    for(auto it = keymap.begin(); it != keymap.end(); it++)
    {
        if(it->second.find(top) != std::string::npos)
        {
            top = it->first;
            it = keymap.begin();
        }
    }
    
    s << top;
    while(exp(keymap, s)){};
    std::cout << s.str().size();
}
