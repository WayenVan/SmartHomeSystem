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
