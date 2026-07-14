#pragma once

#include <Eigen/Dense>
#include <vector>
#include <utility>

class GridMap
{
    private:
    Eigen::MatrixXi grid_;
    int row_ = 0;
    int cols_ = 0;

    public:
    GridMap() =default;
    //X：动态大小；i：int
    explicit GridMap(const Eigen::MatrixXi& grid);
    
    //判断坐标(x,y)是否可通行
    bool isfree(int x, int y) const;
    int rows() const;
    int cols() const;

    void print(const std::vector<std::pair<int, int>>& path = {} ) const;
};

GridMap createSampleMap();