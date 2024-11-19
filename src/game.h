#pragma once

#include <string>

using std::string;
class Game {
    private: 
        string _name;
    
    public:
        Game(string name);

        const string& name();
};