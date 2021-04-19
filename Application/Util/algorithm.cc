#include<opencv2/opencv.hpp>
#include<string>
#include<algorithm.hpp>
#include<vector>

using namespace cv;
using namespace std;

namespace wayenvan{

namespace algorithm{

const string cascade_path = "";

void faceDetect(const Mat& frame, CascadeClassifier& faceCascade, const double& scale, vector<Rect>& faces){
    Mat grayscale;
    cvtColor(frame, grayscale, COLOR_BGR2GRAY);
    equalizeHist(grayscale, grayscale);
    resize(grayscale, grayscale, Size(grayscale.size().width / scale, grayscale.size().height / scale));
    faceCascade.detectMultiScale(grayscale, faces, 1.1, 3, 0, Size(30,30));
}

void drawFaces(Mat& frame, vector<Rect>& faces, const double& scale){
    for(Rect area: faces){
        Scalar drawColor = Scalar(255,0, 0);
        rectangle(frame, Point(cvRound(area.x * scale), cvRound(area.y * scale)),
                    Point(cvRound((area.x + area.width - 1) * scale),
                        cvRound((area.y + area.height - 1) * scale)),drawColor
        );
    }
}

}
}

