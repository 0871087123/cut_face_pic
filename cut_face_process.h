//
// Created by kent on 16-10-10.
//

#ifndef SHOWIMG_CUT_FACE_PROCESS_H
#define SHOWIMG_CUT_FACE_PROCESS_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>


#include "config.h"

using namespace std;

class cut_face_process {
public:
    static vector<string> failed_files;
    static float param_x;
    static int init();
    static IplImage * process_file(std::string filename);
};


#endif //SHOWIMG_CUT_FACE_PROCESS_H
