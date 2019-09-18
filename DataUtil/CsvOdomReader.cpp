//
// Created by akira on 19-9-16.
//

#include "CsvOdomReader.h"
#include <sstream>
#include "iostream"

CsvOdomReader::CsvOdomReader(char *file) {
    this->filename = file;
    this->filestr.open(file);
    char line[1000];
    while (this->filestr.getline(line, 1000)){
        std::string str(line);
        Pose3d<double> pose;
        this->parseLine(str, pose, ',');
        this->poseList.push_back(pose);
    }
    this->frameNum = this->poseList.size();
}

void CsvOdomReader::parseLine(std::string line, Pose3d<double> &pose3d, char sep) {
    std::stringstream strstr;
    strstr << line;
    std::string s;
    double tmp;
    int cnt =  0;
    while(std::getline(strstr, s, sep)){
        std::stringstream strstr2;
        strstr2 << s;
        strstr2 >> tmp;
        cnt++;
        switch(cnt){
            case 1:
                break;
            case 2:
                pose3d.x = tmp;
                break;
            case 3:
                pose3d.y = tmp;
                break;
            case 4:
                pose3d.yaw =  tmp;
                break;
            default:
                break;
        }
    }

    return;

}

bool CsvOdomReader::getFrame(int frminx, Pose3d<double>&pose) {
    if(frminx >= this->frameNum)
        return false;
    pose = this->poseList[frminx];
    return true;
}


