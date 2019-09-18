//
// Created by akira on 19-9-16.
//

#ifndef PROJECT_CSVODOMREADER_H
#define PROJECT_CSVODOMREADER_H
#include "fstream"
#include "pose3d.hpp"
#include "vector"

class CsvOdomReader {
public:
    CsvOdomReader(char *file);
    int frameNum = 0;
    char *filename;

    std::ifstream filestr;
    std::vector<Pose3d<double>> poseList;
    void parseLine(std::string line, Pose3d<double> &pose3d, char sep);

    /**
     *
     * @param frminx >= 0
     * @param pose
     * @return
     */
    bool getFrame(int frminx, Pose3d<double>&pose);
};


#endif //PROJECT_CSVODOMREADER_H
