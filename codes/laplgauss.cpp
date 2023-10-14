#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(cv::Mat &m) {
    for (int i = 0; i < m.size().height; i++) {
        for (int j = 0; j < m.size().width; j++) {
            std::cout << m.at<float>(i, j) << ",";
        }
        std::cout << "\n";
    }
}

int main(int, char **) {
    cv::VideoCapture cap;  // open the default camera
    float media[] = {0.1111, 0.1111, 0.1111, 
                     0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111};
    float gauss[] = {0.0625, 0.1250, 0.0625,
                     0.1250, 0.2500, 0.1250,
                     0.0625, 0.1250, 0.0625};
    float horizontal[] = {-1, 0, 1,
                          -2, 0, 2,
                          -1, 0, 1};
    float vertical[] = {-1,-2,-1,
                         0, 0, 0,
                         1, 2, 1};
    float laplacian[] = { 0,-1, 0,
                         -1, 4,-1,
                          0,-1, 0};
    float boost[] = { 0, -1, 0,
                     -1,5.2,-1, 
                      0, -1, 0};

    cv::Mat frame, framegray, frame32f, frameFiltered;
    std::vector<cv::Mat> mask = {cv::Mat(3, 3, CV_32F), cv::Mat(3,3,CV_32F)};
    cv::Mat result;
    double width, height;
    int absolut;
    char key;
    int qtd_filter = 1;
    std::string nameFilter[2];

    cap.open(0);

    if (!cap.isOpened()){
        std::cout << "Camera indisponivel" << std::endl;
        return -1;
    }  // check if we succeeded

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "largura=" << width << "\n";
    std::cout << "altura =" << height << "\n";

    std::cout << "fps    =" << cap.get(cv::CAP_PROP_FPS) << "\n";
    std::cout << "format =" << cap.get(cv::CAP_PROP_FORMAT) << "\n";

    cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);
    cv::namedWindow("original", cv::WINDOW_NORMAL);

    mask[0] = cv::Mat(3, 3, CV_32F, media);
    nameFilter[0] = "filtroespacial";

    absolut = 1;  // calcs abs of the image
    
    for (;;) {
        cap >> frame;  // get a new frame from camera
        cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
        cv::flip(framegray, framegray, 1);
        cv::imshow("original", framegray);
        framegray.convertTo(frame32f, CV_32F);
        
        for(int i = 0; i < qtd_filter; i++){
            cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask[i], cv::Point(1, 1), 0);
        
        
            if (absolut) {
                frameFiltered = cv::abs(frameFiltered);
            }

            frameFiltered.convertTo(result, CV_8U);
        
            cv::imshow(nameFilter[i], result);

            frameFiltered.copyTo(frame32f);
        } 
        key = (char)cv::waitKey(10);
        if (key == 27) break;  // esc pressed!
        switch (key) {
            case 'a':
                absolut = !absolut;
                break;
            case 'm':
                mask[0] = cv::Mat(3, 3, CV_32F, media);
                printmask(mask[0]);
                nameFilter[0] = "filtroespacial";
                qtd_filter = 1;
                break;
            case 'g':
                mask[0] = cv::Mat(3, 3, CV_32F, gauss);
                printmask(mask[0]);
                nameFilter[0] = "filtroespacial";
                qtd_filter = 1;
                break;
            case 'h':
                mask[0] = cv::Mat(3, 3, CV_32F, horizontal);
                printmask(mask[0]);
                nameFilter[0] = "filtroespacial";
                qtd_filter = 1;
                break;
            case 'v':
                mask[0] = cv::Mat(3, 3, CV_32F, vertical);
                printmask(mask[0]);
                nameFilter[0] = "filtroespacial";
                qtd_filter = 1;
                break;
            case 'l':
                mask[0] = cv::Mat(3, 3, CV_32F, laplacian);
                printmask(mask[0]);
                nameFilter[0] = "filtroespacial";
                qtd_filter = 1;
                break;
            case 'b':
                mask[0] = cv::Mat(3, 3, CV_32F, boost);
                nameFilter[0] = "filtroespacial";
                qtd_filter = 1;
                break;
            case 'e':
                mask[0] = cv::Mat(3, 3, CV_32F, laplacian);
                mask[1] = cv::Mat(3, 3, CV_32F, gauss);
                nameFilter[0] = "filtroespacial";
                nameFilter[1] = "Laplacian - Gauss";
                qtd_filter = 2;
                break;
            default:
                break;
        }
         
    }
    return 0;
}