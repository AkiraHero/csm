#ifndef LASERSCANDATA_H
#define LASERSCANDATA_H
#include <memory>
#include "./laserpoint.hpp"
class LaserScanData
{
public:
    LaserScanData(){
        this->pointNum = 0;
        this->rangeMax = 80.0;
        this->maxPtNum = 3000;
        this->pointList = std::shared_ptr<LaserPoint>(new LaserPoint[this->maxPtNum], [](LaserPoint *p){delete[] p; });};
    float rangeMax;
    int pointNum;
    int maxPtNum;
    std::shared_ptr<LaserPoint> pointList;
//    double ranges[2000];
    double angleres;
    std::pair<double ,double> anglerange;
    long long timestamp;
};

#endif // LASERSCANDATA_H
