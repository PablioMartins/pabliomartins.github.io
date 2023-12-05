#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <numeric>

#define STEP 5
#define JITTER 3
#define RATIO 3

int main(int argc, char **argv){
    cv::Mat image, border, points;
    int width, height, x, y, gray;
    int lowThreshold = 50, ratio = 3;
    std::vector<int> xrange, yrange;

    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if(image.empty()){
        std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    width = image.cols;
    height = image.rows;

    cv::Canny(image, border, lowThreshold, lowThreshold * ratio);
    cv::imwrite("img/border_canny.png", border);
    
    xrange.resize(height / STEP);
    yrange.resize(width / STEP);

    std::iota(xrange.begin(), xrange.end(), 0);
    std::iota(yrange.begin(), yrange.end(), 0);

    for(uint i = 0; i < xrange.size(); i++){
        xrange[i] = xrange[i] * STEP + STEP / 2;
    }
    for(uint i = 0; i < yrange.size(); i++){
        yrange[i] = yrange[i] * STEP + STEP / 2;
    }

    points = cv::Mat(height, width, CV_8U, cv::Scalar(255));

    std::random_shuffle(xrange.begin(), xrange.end());
    for(auto i : xrange){
        std::random_shuffle(yrange.begin(), yrange.end());
        for (auto j: yrange){
            x = i + std::rand() % (2 * JITTER - JITTER + 1);
            y = j + std::rand() % (2 * JITTER - JITTER + 1);
            
            gray = image.at<uchar>(x,y);
            if(border.at<uchar>(x,y) == 255){
                cv::circle(points, cv::Point(y,x), RATIO, CV_RGB(0, 0, 0),
                       cv::FILLED, cv::LINE_AA);
            }
            else {
               cv::circle(points, cv::Point(y,x), RATIO, CV_RGB(gray, gray, gray),
                          cv::FILLED, cv::LINE_AA);
            }
        }
    }
    cv::imwrite("img/cannypoint.png", points);
    return 0;
}