#include "game.h"
#include <vector>
#include <stdexcept>
#include <fmt/core.h>

Game::Game(string name) : _name(name) {}
const string& Game::name() const { 
    return _name; 
}

// const std::vector<Renderable>& Game::get_world() const { 
//     return _world; 
// }
