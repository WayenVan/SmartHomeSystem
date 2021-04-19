#include "opencv2/opencv.hpp"
#include "vector"
#include "stdlib.h"
#include "iostream"

using namespace cv;
using namespace std;

int main() {

    double scale = 3.0;
    CascadeClassifier faceCascade;
    string Path = "/Users/liangyue/Documents/OpenCV/opencv/data/haarcascades/haarcascade_frontalface_default.xml";
    faceCascade.load(Path);

    // Open the video capture
    VideoCapture cap(0);
    if(!cap.isOpened()) {
        cout << "Camera open failed" << endl;
        exit(0);
    }

    else if(!faceCascade.load(Path)) {
        cout << "File is not loaded properly" << endl;
        exit(0);
    }

    else {
        // Keep getting frames from the webcam over and over again
        while (1) {
            // OpenCV images are stored in the mat object - frame
            Mat frame;
            cap >> frame;

            Mat grayscale;
            cvtColor(frame, grayscale, COLOR_BGR2GRAY);
            resize(grayscale, grayscale, Size(grayscale.size().width / scale, grayscale.size().height / scale));

            vector<Rect> faces;
            faceCascade.detectMultiScale(grayscale,faces,1.1,3,0,Size(30,30));

            for(Rect area: faces){
                Scalar drawColor = Scalar(255,0, 0);
                rectangle(frame, Point(cvRound(area.x * scale), cvRound(area.y * scale)),
                          Point(cvRound((area.x + area.width - 1) * scale),
                                cvRound((area.y + area.height - 1) * scale)),drawColor
                );
            }

            imshow("CAMERA FRAME", frame);

            if(waitKey(30) >= 0)
                break;
        }

    }

    return 0;

}

