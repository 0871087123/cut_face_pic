//
// Created by kent on 16-10-10.
//

#include "cut_face_process.h"
#include <stdlib.h>

using namespace std;
using namespace cv;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

string win_name = "Capture";

RNG rng(12345);


vector<string> cut_face_process::failed_files;

int cut_face_process::init() {
    if (!face_cascade.load(face_cascade_name)) {printf("Error Load\n"); return -1;}
    if (!eyes_cascade.load(eyes_cascade_name)) {printf("Error Load\n"); return -1;}
    cut_face_process::failed_files.clear();
    system("mkdir out");
}

IplImage* cut_face_process::process_file(std::string filename) {
    IplImage * img = cvLoadImage(filename.c_str());
    string outfile = "out_";
    char idbuf[40];
    Mat frame(img, false);
    vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(50);

    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
//        ellipse(frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0);
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        Rect face_area = faces[i];
        face_area.width *= 1.5f; // large face detected
        face_area.height = face_area.width / JPG_IMG_SCALE_VERTICAL * JPG_IMG_SCALE_HORIZEN;
        face_area.x = center.x - (face_area.width / 2);
        face_area.y = center.y - (face_area.height / 2);
        if (((face_area.x + face_area.width) > frame.cols) ||
            (face_area.x < 0) ||
            (face_area.y < 0) ||
            (face_area.y + face_area.height) > frame.rows)
        {
            cut_face_process::failed_files.push_back(filename);
            return NULL;
        }
        Mat face(frame, face_area);
        Mat outface;
        resize(face, outface, Size(JPG_IMG_WIDTH, JPG_IMG_HEIGHT));
        sprintf(idbuf, "%d", i);
        string outfilename = outfile + idbuf + "_" + filename;
        imwrite(outfilename, outface, params);
        string cmd = "mv ";
        system((cmd + outfilename + " out/").c_str());

//        //-- In each face, detect eyes
//        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
//        for( size_t j = 0; j < eyes.size(); j++ )
//        {
//            Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
//            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//            circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
//        }
    }


    return NULL;
}
