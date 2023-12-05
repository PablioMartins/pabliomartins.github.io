#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
    cv::Mat image_in, image_out, str_v, str_h;

    if(argc != 2){
        std::cout << "Parametros de entrada incorreto" << std::endl;
        return -1;
    }

    image_in = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    if(image_in.empty()){
        std::cout << "Erro na leitura da imagem." << std::endl;
        return -1;
    }

    str_v = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,15));
    str_h = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,1));
    
    cv::erode(image_in, image_out, str_v);
    cv::erode(image_out, image_out, str_h);
    cv::dilate(image_out, image_out, str_v);

    cv::imwrite("img/digitos__morf.png", image_out);
    cv::waitKey();
    return 0;
}