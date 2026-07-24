#pragma once

#include "obstacle_map.h"
#include <vector>
#include <utility>

// A* 搜索接口
// 返回值：从起点到终点的坐标序列（包含起止点），若不存在则返回空 vector
std::vector<std::pair<int, int>>
aStarSearch(const GridMap& map,
            std::pair<int, int> start,
            std::pair<int, int> goal);