#include "obstacle_map.h"
#include <queue>
#include <cmath>
#include <memory>
#include <unordered_set>
#include <vector>
#include <algorithm>

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

//比较f大小判断最优路线
struct CompareF
{
    bool operator()(const std::shared_ptr<AStarNode>& a, 
                    const std::shared_ptr<AStarNode>& b) const
                    {
                        return a ->f > b ->f;
                        //当 a->f > b->f 时，函数返回 true，
                        //优先队列认为 a 的优先级低于 b，因此 b（更小的 f）会被放在更靠近堆顶的位置
                        //最终堆顶就是 f 最小的节点
                    }
};

//用于unordered_set的哈希值
struct PairHash
{
    std::size_t operator()(const std::pair<int, int>& p) const
    {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

//曼哈顿距离
static inline double heuristic(int x1, int y1, int x2, int y2) 
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

//路径回溯
static std::vector<std::pair<int, int>>
reconstructPath(const std::shared_ptr<AStarNode>& goalNode) {
    std::vector<std::pair<int, int>> path;
    auto node = goalNode;
    while (node) {
        //emplace_back 直接在 vector 的末尾原地构造一个 pair<int,int>
        //终点 → 倒数第二个节点 → … → 起点
        path.emplace_back(node->x, node->y);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// ------------------------------------------------------------
// 公开接口
// ------------------------------------------------------------

std::vector<std::pair<int,int>>
aStarSearch(const GridMap& map,
            std::pair<int,int> start,
            std::pair<int,int> goal)
            {
               const int rows = map.rows();
               const int cols = map.cols();
               
            }