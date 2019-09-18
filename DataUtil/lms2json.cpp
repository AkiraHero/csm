//
// Created by akira on 19-9-16.
//
extern "C" {
#include "../sm/csm/laser_data_json.h"
}
#include "lmsfilereader.h"
#include "CsvOdomReader.h"

int main(){
    FILE *Fstr;
    Fstr = fopen("sampledlms.lms.json", "w");
    char *odomfile = "/home/akira/poss-server/dataprocessing/corridor0416/traj_noisyodometer.csv";
    char *lmsfile = "/home/akira/poss-server/dataprocessing/corridor0416/sampledlms.lms";
    int praynum = 1800;
    //corridor dta: 30.0m
    //campus dta : 99.0m
    float maxdist = 30.0;


    CsvOdomReader csvOdomReader(odomfile);
    LmsFileReader lmsFileReader(lmsfile);
    lmsFileReader.open();
    int framenum = lmsFileReader.totalFrames;
    assert(framenum == csvOdomReader.frameNum);

    LaserScanData framedata;
    Pose3d<double> odominfo;
    LDP data;
    ld_alloc(data, praynum);
    data->min_theta = 0.0;
    data->max_theta = lmsFileReader.angleResolution * (praynum - 1) / 360.0 * 2 * M_PI;

    for(int i = 0; i != framenum; i++){
        lmsFileReader.getData(i + 1, framedata);
        csvOdomReader.getFrame(i, odominfo);
        data->odometry[0] = odominfo.x;
        data->odometry[1] = odominfo.y;
        data->odometry[2] = odominfo.yaw;
        for(int j = 0; j != praynum; j++){
            if(framedata.pointList.get()[j].dist < 1e-9 || framedata.pointList.get()[j].dist > maxdist) {
                data->valid[j] = 0;
                data->theta[j] = framedata.angleres * j / 360.0 * 2 * M_PI;
                data->readings[j] = framedata.pointList.get()[j].dist;
            }
            else{
                data->theta[j] = framedata.angleres * j / 360.0 * 2 * M_PI;
                data->readings[j] = framedata.pointList.get()[j].dist;
                data->valid[j] = 1;
            }

        }
        data->max_theta = framedata.angleres * (praynum - 1) / 360.0 * 2 * M_PI;
        ld_write_as_json(data, Fstr);

    }
    fclose(Fstr);
    return 0;



}
