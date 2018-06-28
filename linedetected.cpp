#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.1415926
#define VIDEOPATH "/home/yolo/Videos/test.m4v"

static void getThreshold( int, void*)
{

}
int main()
{
        cv::VideoCapture cap(VIDEOPATH); // open the default camera
        if(!cap.isOpened()) // check if we succeeded
                return -1;

        cv::Mat image;
        cap >> image;

        int lowThreshold, highThreshold;

        cv::namedWindow("Original Image");
        cv::namedWindow("Detected Lines with Hough");
        cv::namedWindow("result");
        //트랙바를 생성하고 디폴트값으로 50지정
        cv::createTrackbar("min threahold", "result", &lowThreshold, 300, getThreshold );
        cv::setTrackbarPos("min threahold", "result", 50);

        //트랙바를 생성하고 디폴트값으로 150지정
        cv::createTrackbar("max threahold", "result", &highThreshold, 300, getThreshold );
        cv::setTrackbarPos("max threahold", "result", 150);

        cv::Mat oriimg;
        oriimg=image.clone();

        while(1) {

                lowThreshold = cv::getTrackbarPos("min threahold", "result");
                highThreshold = cv::getTrackbarPos("max threahold", "result");
                //cv::Mat image= cv::imread("road.jpg", 0);

                cv::imshow("Original Image",oriimg);

                image=oriimg.clone();

                // 캐니 알고리즘 적용
                cv::Mat contours;
                cv::Canny(image, contours, lowThreshold, highThreshold);

                // 선 감지 위한 허프 변환
                std::vector<cv::Vec2f> lines;
                cv::HoughLines(contours, lines,
                               1,PI/180, // 단계별 크기
                               80); // 투표(vote) 최대 개수

                // 선 그리기

                cv::Mat result(contours.rows, contours.cols, CV_8U, cv::Scalar(255,255,225));
                std::cout << "Lines detected: " << lines.size() << std::endl;

                // 선 벡터를 반복해 선 그리기
                std::vector<cv::Vec2f>::const_iterator it= lines.begin();
                while (it!=lines.end()) {
                        float rho = (*it)[0]; // 첫 번째 요소는 rho 거리
                        float theta = (*it)[1]; // 두 번째 요소는 델타 각도
                        if (theta < PI/4. || theta > 3.*PI/4.) { // 수직 행
                                cv::Point pt1(rho/cos(theta), 0); // 첫 행에서 해당 선의 교차점
                                cv::Point pt2((rho-result.rows*sin(theta))/cos(theta), result.rows);
                                // 마지막 행에서 해당 선의 교차점
                                cv::line(image, pt1, pt2, cv::Scalar(255,255,255), 1); // 하얀 선으로 그리기

                        } else { // 수평 행
                                cv::Point pt1(0,rho/sin(theta)); // 첫 번째 열에서 해당 선의 교차점
                                cv::Point pt2(result.cols,(rho-result.cols*cos(theta))/sin(theta));
                                // 마지막 열에서 해당 선의 교차점
                                cv::line(image, pt1, pt2, cv::Scalar(255,255,255), 1); // 하얀 선으로 그리기
                        }
                        std::cout << "line: (" << rho << "," << theta << ")\n";
                        ++it;
                        //

                }


                cv::imshow("Detected Lines with Hough",image);
                int key = cv::waitKey(1);
                if (key == 'q') break;
        }


        return 0;
}
