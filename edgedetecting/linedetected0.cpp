#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;
#define PI 3.1415926
#define VIDEOPATH "/home/yolo/Videos/right.MP4"
#define VIDEOOUTPATH "/home/yolo/Videos/result/right.MP4"

#define SAMPLERATE 2
#define SAMPLERATIO 2


static void getThreshold( int, void*)
{

}



int main()
{
        VideoCapture cap(VIDEOPATH); // open the default camera
        if(!cap.isOpened()) // check if we succeeded
                return -1;

        Mat image,resultimg;
        cap >> image;



        cap.set(CAP_PROP_FPS, 30);

        VideoWriter outputVideo;
        outputVideo.open(VIDEOOUTPATH, VideoWriter::fourcc('M', 'J', 'P', 'G'),
                         240/SAMPLERATE, Size(image.cols/SAMPLERATIO, image.rows/SAMPLERATIO), false);

        if (!outputVideo.isOpened())
        {
                cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;
                return 1;
        }

        //int lowThreshold, highThreshold;

        namedWindow("Original Image");
        namedWindow("Detected Lines with Hough");

        /*
           namedWindow("result");
           //트랙바를 생성하고 디폴트값으로 50지정
           createTrackbar("min threahold", "result", &lowThreshold, 300, getThreshold );
           setTrackbarPos("min threahold", "result", 50);

           //트랙바를 생성하고 디폴트값으로 150지정
           createTrackbar("max threahold", "result", &highThreshold, 300, getThreshold );
           setTrackbarPos("max threahold", "result", 150);
         */


        Mat background = Mat::zeros(Size(image.cols/SAMPLERATIO, image.rows/SAMPLERATIO),CV_8U);

        int i;


        while(cap.isOpened()) {

                for(i=0; i<SAMPLERATE; i++) {
                        cap >> image;

                        if(image.cols<=0){
                          break;
                        }
                        //cout << image.cols << "\n";
                }
                resultimg=background.clone();

                if(image.cols<=0){
                  break;
                }
                resize(image,image,Size(image.cols/SAMPLERATIO, image.rows/SAMPLERATIO), 0, 0, CV_8U);
                imshow("Original Image",image);

                // 캐니 알고리즘 적용
                Mat contours;
                Canny(image, contours, 100, 200);

                // 선 감지 위한 허프 변환
                vector<Vec2f> lines;
                HoughLines(contours, lines,
                           1,PI/180,     // 단계별 크기
                           80);     // 투표(vote) 최대 개수

                // 선 그리기

                Mat result(contours.rows, contours.cols, CV_8U, Scalar(255,255,225));
                cout << "Lines detected: " << lines.size() << endl;

                // 선 벡터를 반복해 선 그리기
                vector<Vec2f>::const_iterator it= lines.begin();
                while (it!=lines.end()) {
                        float rho = (*it)[0]; // 첫 번째 요소는 rho 거리
                        float theta = (*it)[1]; // 두 번째 요소는 델타 각도
                        if (theta < PI/4. || theta > 3.*PI/4.) { // 수직 행
                                Point pt1(rho/cos(theta), 0); // 첫 행에서 해당 선의 교차점
                                Point pt2((rho-result.rows*sin(theta))/cos(theta), result.rows);
                                // 마지막 행에서 해당 선의 교차점
                                line(resultimg, pt1, pt2, Scalar(255,255,255), 1); // 하얀 선으로 그리기

                        } else { // 수평 행
                                Point pt1(0,rho/sin(theta)); // 첫 번째 열에서 해당 선의 교차점
                                Point pt2(result.cols,(rho-result.cols*cos(theta))/sin(theta));
                                // 마지막 열에서 해당 선의 교차점
                                line(resultimg, pt1, pt2, Scalar(255,255,255), 1); // 하얀 선으로 그리기
                        }
                        cout << "line: (" << rho << "," << theta << ")\n";
                        ++it;


                }


                imshow("Detected Lines with Hough",resultimg);
                outputVideo.write(resultimg);

                int key = waitKey(1);
                if (key == 'q') break;
        }

        cap.release();
        outputVideo.release();
        destroyAllWindows();
        return 0;
}
