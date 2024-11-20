#pragma once

#include <string>
#include <vector>
#include "engine/mesh/primitives.h"

using std::string;
class Game {
    private: 
        string _name;
        // Engine engine;

        // std::vector<Renderable> _world;
    
    public:
        Game(string name);

        const string& name() const;
        // const std::vector<Renderable>& get_world() const;
};