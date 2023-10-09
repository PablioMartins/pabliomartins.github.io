#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**){
    cv::Mat image;

    image = cv::imread("./img/biel.png", cv::IMREAD_GRAYSCALE);
    if(!image.data){
        std::cout << "Nao foi possivel abrir a imagem" << std::endl;
        return 0;
    }

    int image_height = image.rows;
    int image_width = image.cols;

    cv::Mat image_result;
    image_result = cv::Mat::zeros(image_height, image_width, CV_8UC1);
    
    image(cv::Rect(0,0,image_width/2, image_height/2)).copyTo(image_result(cv::Rect(image_width/2, image_height/2,image_width/2, image_height/2)));
    image(cv::Rect(image_width/2, image_height/2,image_width/2, image_height/2)).copyTo(image_result(cv::Rect(0, 0,image_width/2, image_height/2)));
    image(cv::Rect(image_width/2, 0,image_width/2, image_height/2)).copyTo(image_result(cv::Rect(0, image_width/2,image_width/2, image_height/2)));
    image(cv::Rect(0, image_height/2,image_width/2, image_height/2)).copyTo(image_result(cv::Rect(image_width/2, 0,image_width/2, image_height/2)));

    cv::imshow("ReplaceRegions", image_result);
    cv::imwrite("./resultimg/repleaceregions.png", image_result);
    cv::waitKey();

    return 0;
}