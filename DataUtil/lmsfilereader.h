#ifndef LMSFILEREADER_H
#define LMSFILEREADER_H
#include <fstream>
#include "./laserscandata.h"

class LmsFileReader
{
public:
    LmsFileReader(char *filename);
    LmsFileReader();
    char *file;
    std::ifstream fileStream;
    int currentframe;
    int totalFrames;
    float angleRange;
    float angleResolution;
    float disUnit;
    int framePointNum;
    int headBytesNum;
    int frameBytesNum;
    long long totalBytes;
    float rangeMax;
    bool isOpen;
    void setFilePath(char *file);
    bool isOpened();
    void open();
    int calTotalFrames();
    void jump2Frame(int frame);
    void getData(LaserScanData &data);
    void getData(int frame, LaserScanData &data);
    void close();
};

#endif // LMSFILEREADER_H
