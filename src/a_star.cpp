#include "obstacle_map.h"
#include <queue>
#include <cmath>
#include <memory>
#include <unordered_set>
#include <vector>

struct AStarNode //默认是public
{
    int x,y;
    double g,h;
    std::shared_ptr<AStarNode> parent;
    double f;

    AStarNode(int x, int y, double g, double h,
              std::shared_ptr<AStarNode> parent = nullptr)
        :x(x), y(y), g(g), h(h), f(g+h), parent(std::move(parent)) {}
    //这里使用了std::move将智能指针所有权转给结构体里面的parent
};