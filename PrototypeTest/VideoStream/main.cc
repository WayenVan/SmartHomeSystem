#include <Camera.hpp>
#include <share_print.hpp>

#include <string>

#include <opencv2/core.hpp>


using namespace std;

int main (){
    Camera* cam = Camera::getInstance();
    cv::Mat frame = cam->getFrame();
    share_print(std::to_string(frame.dims));

    return 1;
}