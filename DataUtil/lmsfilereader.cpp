#include "lmsfilereader.h"
//#include "QDebug"
//#include "QFile"
#include "cmath"
#include <stdio.h>
LmsFileReader::LmsFileReader(char *filename)
{
    this->file = filename;
    this->rangeMax = 80.0;
    this->isOpen = false;
}

LmsFileReader::LmsFileReader()
{
    this->rangeMax = 80.0;
    this->isOpen = false;
}

void LmsFileReader::setFilePath(char *file)
{
    this->file = file;
}

bool LmsFileReader::isOpened()
{
    return isOpen;
}

void LmsFileReader::open()
{
    this->fileStream.open(this->file, std::ios_base::binary);
    this->isOpen = this->fileStream.is_open();
    float tmp[3];
    this->fileStream.read((char*)tmp, sizeof(float) * 3);
    this->angleRange = tmp[0];
    this->angleResolution = tmp[1];
    this->disUnit = tmp[2];
    this->framePointNum = int((double)this->angleRange / (double)this->angleResolution) + 1;
    this->calTotalFrames();
}

int LmsFileReader::calTotalFrames()
{
    if(!this->isOpen)
        return 0;
    this->headBytesNum = 12;
    this->frameBytesNum = 4 + 2 * this->framePointNum;
//    this->totalBytes = QFile(this->file).size();
    auto curPos = this->fileStream.tellg();
    this->fileStream.seekg(0, std::ios_base::end);
    this->totalBytes = this->fileStream.tellg();
    this->fileStream.seekg(curPos, std::ios_base::beg);
    this->totalFrames = int((totalBytes - headBytesNum)/frameBytesNum);
}

void LmsFileReader::jump2Frame(int frame)
{
    int pos = this->headBytesNum + (frame - 1) * this->frameBytesNum;
    this->fileStream.seekg(pos, std::ios_base::beg);
}

void LmsFileReader::getData(LaserScanData &data)
{
    short *buffer = new short[this->framePointNum];
    int timestamp = 0;
    this->fileStream.read((char*)&timestamp, sizeof(int));
    this->fileStream.read((char*)buffer, sizeof(short) * this->framePointNum);
    data.pointNum = this->framePointNum;
    data.rangeMax = this->rangeMax;
    data.angleres = angleResolution;
    data.anglerange = std::make_pair(0.0, angleRange);

    data.timestamp = timestamp;
    for (int i = 0; i != this->framePointNum; i++){
        LaserPoint pt;
        float dist = buffer[i] / this->disUnit;
        pt.dist = dist;
        pt.timeStamp = timestamp;
        if (pt.dist <= data.rangeMax)
            pt.valid = true;
        else
            pt.valid = false;
        pt.angle = this->angleResolution * i / 180.0 * M_PI;
        pt.x = pt.dist * cos(pt.angle);
        pt.y = pt.dist * sin(pt.angle);
        data.pointList.get()[i] = pt;
//        data.ranges[i] = pt.dist;
    }
}

/**
 *
 * @param frame >=1
 * @param data
 */
void LmsFileReader::getData(int frame, LaserScanData &data)
{
    auto iniPos = this->fileStream.tellg();
    this->jump2Frame(frame);
    this->getData(data);
    this->fileStream.seekg(iniPos, std::ios_base::beg);
}

void LmsFileReader::close()
{
    this->fileStream.close();
    this->isOpen = false;
}
