#include "game.h"

Game::Game(string name) : _name(name) {}
const string& Game::name() { return _name; }