#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**){
    cv::Mat image;
    cv::Vec3b val;

    image = cv::imread("./img/biel.png", cv::IMREAD_GRAYSCALE);
    if(!image.data){
        std::cout << "Nao foi possivel abrir a imagem" << std::endl;
        return 0;
    }

    cv::Vec2i first_point;
    cv::Vec2i second_point;

    do{
        std::cout << "Digite a coordenada 'x' do primero ponto (0 a " <<  image.rows << ")" << std::endl;
        std::cin >> first_point[0];
        if(first_point[0] < 0 || first_point[0] >= image.rows)
            std::cout << "O valor deve ser entre 0 e " << image.rows << std::endl;
    } while (first_point[0] < 0 || first_point[0] >= image.rows);

    do{
        std::cout << "Digite a coordenada 'y' do primero ponto (0 a " <<  image.cols << ")" << std::endl;
        std::cin >> first_point[1];
        if(first_point[1] < 0 || first_point[1] >= image.cols)
            std::cout << "O valor deve ser entre 0 e " << image.cols << std::endl;
    } while (first_point[1] < 0 || first_point[1] >= image.cols);

    do{
        std::cout << "Digite a coordenada 'x' do segundo ponto (0 a " <<  image.rows << ")" << std::endl;
        std::cin >> second_point[0];
        if(second_point[0] < 0 || second_point[0] >= image.rows)
            std::cout << "O valor deve ser entre 0 e " << image.rows << std::endl;
    } while (second_point[0] < 0 || second_point[0] >= image.rows);

    do{
        std::cout << "Digite a coordenada 'y' do primero ponto (0 a " <<  image.cols << ")" << std::endl;
        std::cin >> second_point[1];
        if(second_point[1] < 0 || second_point[1] >= image.cols)
            std::cout << "O valor deve ser entre 0 e " << image.cols << std::endl;
    } while (second_point[1] < 0 || second_point[1] >= image.cols);

    int x_smaller, x_bigger, y_smaller, y_bigger;
    if(first_point[0] < second_point[0]){
        x_smaller = first_point[0];
        x_bigger = second_point[0];
    } 
    else {
        x_smaller = second_point[0];
        x_bigger = first_point[0];
    }

    if(first_point[1] < second_point[1]){
        y_smaller = first_point[1];
        y_bigger = second_point[1];
    } 
    else {
        y_smaller = second_point[1];
        y_bigger = first_point[1];
    }

    for( int i = x_smaller; i < x_bigger; i++){
        for(int j = y_smaller; j < y_bigger; j++){
            image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
        }
    }
    
    cv::imshow("Regions", image);
    cv::imwrite("./resultimg/regions.png", image);
    cv::waitKey();
    return 0;
}

