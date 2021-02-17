//Include OpenCV
#include <opencv2/opencv.hpp>

int main(void )
{
    //Capture stream from webcam.
    cv::VideoCapture capture(0);

    //Check if we can get the webcam stream.
    if(!capture.isOpened())
    {
        std::cout << "Could not open camera" << std::endl;
        return -1;
    }

    //OpenCV saves detection rules as something called a CascadeClassifier which
    //    can be used to detect objects in images.
    cv::CascadeClassifier faceCascade;

    //We'll load the lbpcascade_frontalface.xml containing the rules to detect faces.
    //The file should be right next to the binary.
    if(!faceCascade.load("lbpcascade_frontalface.xml"))
    {
        std::cout << "Failed to load cascade classifier" << std::endl;
        return -1;
    }

    while (true)
    {
        //This variable will hold the image from the camera.
        cv::Mat cameraFrame;

        //Read an image from the camera.
        capture.read(cameraFrame);

        //This vector will hold the rectangle coordinates to a detection inside the image.
        std::vector<cv::Rect> faces;

        //This function detects the faces in the image and
        // places the rectangles of the faces in the vector.
        //See the detectMultiScale() documentation for more details
        // about the rest of the parameters.
        faceCascade.detectMultiScale(
                cameraFrame,
                faces,
                1.09,
                3,
                0 | cv::CASCADE_SCALE_IMAGE,
                cv::Size(30, 30));

        //Here we draw the rectangles onto the image with a red border of thikness 2.
        for( size_t i = 0; i < faces.size(); i++ )
            cv::rectangle(cameraFrame, faces[i], cv::Scalar(0, 0, 255), 2);

        //Here we show the drawn image in a named window called "output".
        cv::imshow("output", cameraFrame);

        //Waits 50 miliseconds for key press, returns -1 if no key is pressed during that time
        if (cv::waitKey(50) >= 0)
            break;
    }

    return 0;
}
