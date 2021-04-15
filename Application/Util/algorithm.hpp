#ifndef _ALGORITHM_HPP_
#define _ALGORITHM_HPP_

#include<opencv2/opencv.hpp>
#include<vector>    

namespace wayenvan{
namespace algorithm{

void faceDetect(const cv::Mat& frame, cv::CascadeClassifier& faceCascade, const double& scale, std::vector<cv::Rect>& faces);
void drawFaces(cv::Mat& frame, std::vector<cv::Rect>& faces, const double& scale);

}

}
#endif
