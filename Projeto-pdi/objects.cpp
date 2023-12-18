#include <opencv2/opencv.hpp>
#include <iostream>

enum StatesBorder {T1, T2, T3, T4, T5, T7, T8};

StatesBorder operator+(StatesBorder state, int increment){
    return static_cast<StatesBorder>((static_cast<int>(state) + increment) % 8);
}

std::vector<std::pair<int, int>> desloc = {
        { 0, -1}, // T1
        {-1, -1}, // T2
        {-1,  0}, // T3
        {-1,  1}, // T4
        { 0,  1}, // T5
        { 1,  1}, // T6
        { 1,  0}, // T7
        { 1, -1}  // T8
};

void extract_object(cv::Mat image_input , cv::Mat *visited, cv::Point2d p, int *numObject){
    int x_low, y_low, x_hight, y_hight, x_current, y_current, point;
    std::string file_name;

    StatesBorder state {T2};

    x_current = x_low = x_hight = p.x;
    y_current = y_low = y_hight = p.y;

    int neighborhood_check = 0;

    while (1)
    {       
        point = image_input.at<uchar>(x_current + desloc[state].first, y_current + desloc[state].second);
        
        if(point == 0){    
            
            x_current = x_current + desloc[state].first;
            y_current = y_current + desloc[state].second;

            if((*visited).at<uchar>(x_current, y_current) != 0){
                break;
            }

            if(x_current < x_low) x_low = x_current;
            if(y_current < y_low) y_low = y_current;
            if(x_current > x_hight) x_hight = x_current;
            if(y_current > y_hight) y_hight = y_current;

            (*visited).at<uchar>(x_current, y_current) = 1;

            if(state == T1 || state == T3 || state == T5 || state == T7){
                state = state + 7;
            } else {
                state = state + 6;
            }
            neighborhood_check = 0;

        } else {
            neighborhood_check++;
            state = state + 1;
        }
        if(neighborhood_check == 8){
            return;
        }
        
    }
    
    for(int i = x_low; i <= x_hight; i++){
        for(int j = y_low; j <= y_hight; j++){
            (*visited).at<uchar>(i, j) = 1;
        }
    }

    file_name = "img/letter_";
    file_name.append(std::to_string(*numObject));
    file_name.append(".png");

    std::cout << file_name << std::endl;
    (*numObject)++;

    cv::imwrite(file_name, image_input(cv::Rect(y_low-1, x_low-1, y_hight - y_low+3, x_hight - x_low+3)));
}

void cut_out_object(cv::Mat image_binary_input){
    cv::Mat visited(image_binary_input.rows, image_binary_input.cols, CV_8U, cv::Scalar(0));

    int num_object_current=0;

    for(int i=1; i < image_binary_input.rows-1; i++){
        for(int j=1; j < image_binary_input.cols-1; j++){
            if(visited.at<uchar>(i,j) == 0){
                visited.at<uchar>(i,j) = 1;
                if(image_binary_input.at<uchar>(i,j) == 0){
                    extract_object(image_binary_input , &visited, cv::Point2d(i, j), &num_object_current);
                }
            }
        }
    }

}

int main(int argc, char** argv){
    cv::Mat image_gray, image_binary;
    double limiar = 128.0;
    double valMax = 255.0;

    image_gray = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (image_gray.empty()) {
        std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    cv::threshold(image_gray, image_binary, limiar, valMax, cv::THRESH_BINARY);
    
    //cv::imwrite("img/letters_binary.png", image_binary);

    cut_out_object(image_binary);

    return 0;
}