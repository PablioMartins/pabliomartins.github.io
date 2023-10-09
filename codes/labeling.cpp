#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
    cv::Mat image;
    int width, height;
    int numberObjects, bubbles, donut;

    cv::Point pointWhite;

    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if(!image.data) {
        std::cout << "Imagem nao foi carregada corretamente\n";
        return -1;
    }

    width = image.cols;
    height = image.rows;

    std::cout << width << "x" << height << std::endl;

    pointWhite.x = 0;
    pointWhite.y = 0;

    for(int i = 0; i < height; i++){        
        if(image.at<uchar>(i,0) == 255){
            pointWhite.x = 0;
            pointWhite.y = i;
            cv::floodFill(image, pointWhite, 0);
        }
        if(image.at<uchar>(i, height-1) == 255){
            pointWhite.x = height-1;
            pointWhite.y = i;
            cv::floodFill(image, pointWhite, 0);
        }
    }

    for(int i = 0; i < width; i++){
        if(image.at<uchar>(0,i) == 255){
            pointWhite.x = i;
            pointWhite.y = 0;
            cv::floodFill(image, pointWhite, 0);
        }
        if(image.at<uchar>(width-1, i) == 255){
            pointWhite.x = i;
            pointWhite.y = width-1;
            cv::floodFill(image, pointWhite, 0);
        }
    }

    cv::imwrite("./resultimg/labeling_borda.png", image);

    numberObjects = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image.at<uchar>(i, j) == 255){
                numberObjects++;
                pointWhite.x = j;
                pointWhite.y = i;
                cv::floodFill(image, pointWhite, numberObjects);
            }
        }
    }
    cv::imwrite("./resultimg/labeling.png", image);

    cv::floodFill(image, cv::Point(0,0), 255);

    cv::imwrite("./resultimg/labeling_fundo.png", image);

    donut = 0;
    for(int i = 0; i < height; i++){
        for(int j = 1; j < width; j++){
            if(image.at<uchar>(i, j) == 0){
                if(image.at<uchar>(i,j-1) != 255){
                    donut++;
                    cv::floodFill(image, cv::Point(j,i), 255);
                    cv::floodFill(image, cv::Point(j-1,i), 255);
                }
                else{
                    cv::floodFill(image, cv::Point(j,i), 255);
                }           
            }
        }
    }
    cv::imwrite("./resultimg/labeling_no_donut.png", image);

    bubbles = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image.at<uchar>(i, j) != 255){
                bubbles++;
                cv::floodFill(image, cv::Point(j,i), 255);      
            }
        }
    }

    std::cout << "Total de Objetos: " << numberObjects << std::endl;
    std::cout << "Total de Rosquinha: " << donut << std::endl;
    std::cout << "Total de Bolhas: " << bubbles << std::endl;
    
    cv::waitKey();
    return 0;
}