#include "a_star.h"
#include <queue>
#include <cmath>
#include <memory>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <utility>

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

//路径回溯就是回顾完整路径的坐标
static std::vector<std::pair<int, int>>reconstructPath
(const std::shared_ptr<AStarNode>& goalNode) {
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
               
                //四邻域移动方向
                const std::vector<std::pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
            

                // 优先队列，起名为openlist
                // 参数一：元素类型
                // 参数二：容器，比如std::vector
                // 参数三：比较器类型，这里选用的是之前写的CompareF

                std::priority_queue<std::shared_ptr<AStarNode>,
                                    std::vector<std::shared_ptr<AStarNode>>,
                                    CompareF> openList;

                // 定义gScore：起点到每个格子的最小代价
                //初始除了起点为0（后面实现）之外都设置为无穷大
                Eigen::MatrixXd gScore = Eigen::MatrixXd::Constant(rows, cols, INFINITY);
                
                
                // 定义closed 集合：已经扩展过的坐标
                std::unordered_set<std::pair<int, int>, PairHash> closed;

                //起点
                auto startNode = std::make_shared<AStarNode>
                (start.first, start.second, 0.0, 
                 heuristic(start.first, start.second, goal.first, goal.second));
                openList.push(startNode);
                gScore(start.first, start.second) = 0.0;


                while(!openList.empty())
                {
                    //取出当前的最佳节点
                    auto current = openList.top();

                    openList.pop();

                    int cx = current->x;
                    int cy = current->y;
                    
                    // 跳过已关闭或 g 值不是最优的节点
                    if (closed.count({cx, cy}) || current->g > gScore(cx, cy))
                    continue;

                    closed.insert({cx, cy});

                    //如果是终点
                    if(cx == goal.first && cy ==goal.second)
                    {
                        return reconstructPath(current);
                    }

                    //邻居
                    for(const auto& d : dirs)
                    {
                        int nx = cx + d.first;
                        int ny = cy + d.second;

                        if(!map.isFree(nx,ny) || closed.count({nx, ny}))
                            continue;
                        
                        //设定移动代价为1
                        //移动代价 g 回随着距离增加而递增，主要是用了限制路径的长短
                        //方向代价 h 主要使用曼哈顿距离来确定，负责引导方向
                        double tentative_g = current -> g + 1.0;

                        if(tentative_g < gScore(nx ,ny))
                        {
                            gScore(nx, ny) = tentative_g;
                            double h = heuristic(nx, ny, goal.first, goal.second);
                            //传入新的权重和父节点坐标
                            //注意共享指針只是不同地址指向相同的对象，不是共享相同的数据
                            //它会在堆区分配一块全新的内存空间
                            //这就保证了数据不会像正常指针一样被覆盖
                            //可以一层层的回去
                            auto neighbor = std::make_shared<AStarNode>(nx, ny, tentative_g, h, current);
                            openList.push(neighbor);
                        }
                    }
                }

                //若无路经
                return {};
            }
