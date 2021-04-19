/* 
 * This file is part of the SmartHomeSystem distribution (https://github.com/WayenVan/SmartHomeSystem).
 * Copyright (c) 2021 Jingyan Wang.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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

