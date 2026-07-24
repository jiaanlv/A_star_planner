#include <iostream>
#include "a_star.h"

int main() {
    // 创建示例地图
    GridMap map = createSampleMap();

    std::pair<int, int> start = {0, 0};
    std::pair<int, int> goal  = {29, 29};

    std::cout << "障碍地图（空白可通行，██ 障碍）：\n";
    map.print();
    std::cout << "\n起点: (" << start.first << ", " << start.second << ")\n";
    std::cout << "终点: (" << goal.first << ", " << goal.second << ")\n\n";

    // 运行 A*
    auto path = aStarSearch(map, start, goal);

    if (path.empty()) {
        std::cout << "未找到可行路径！\n";
    } else {
        std::cout << "找到路径，步数: " << path.size() - 1 << "\n";
        std::cout << "路径坐标: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << "(" << path[i].first << "," << path[i].second << ")";
            if (i != path.size() - 1) std::cout << " -> ";
        }
        std::cout << "\n\n路径可视化(** 表示路径):\n";
        map.print(path);
    }

    return 0;
}