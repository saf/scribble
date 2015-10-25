#ifndef BASICS_H_
#define BASICS_H_

#include <memory>
#include <set>
#include <vector>

class Tile;
class Player;

using Players = std::vector<std::unique_ptr<Player>>;
using Tiles = std::vector<std::shared_ptr<Tile>>;
using Tileset = std::set<std::shared_ptr<Tile>>;
using Rack = Tiles;
using Bag = Tileset;

#endif
