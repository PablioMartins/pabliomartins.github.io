#include <iostream>
#include <opencv2/opencv.hpp>

void histogram_computation(cv::Mat imageInput, int *histogram){
    for(int i = 0; i < 256; i++){
        histogram[i] = 0;
    }
    for(int i = 0; i < imageInput.rows; i++){
        for(int j = 0; j < imageInput.cols; j++){
            histogram[imageInput.at<uchar>(i,j)] += 1;
        }
    }
}

void histogram_equaliation(cv::Mat imageInput, cv::Mat *imageOutput){
    int histogram[256], histogram_accumulate[256];
    float alpha = 255.0/(imageInput.cols * imageInput.rows);

    histogram_computation(imageInput, histogram);
    histogram_accumulate[0] = alpha * histogram[0];
    for(int i = 1; i < 256; i++){
        histogram_accumulate[i] = histogram_accumulate[i-1] + (alpha * histogram[i]);
    }
    
    for(int i = 0; i < imageInput.rows; i++){
        for(int j = 0; j < imageInput.cols; j++){
            (*imageOutput).at<uchar>(i,j) = histogram_accumulate[imageInput.at<uchar>(i,j)];
        }
    }
}

int main(int argc, char** argv){
    cv::Mat image, image_gray;
    int width, height;
    cv::VideoCapture cap;
    cv::Mat histGray, histGrayEq;
    int nbins = 256;
    float range[] = {0, 255};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    int key;

    cap.open(0);
  
    if(!cap.isOpened()){
        std::cout << "cameras indisponiveis";
        return -1;
    }
  
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins/2;
    
    cv::Mat histImgGray(histh, histw, CV_8UC1, cv::Scalar(0));
    cv::Mat histImgGrayEq(histh, histw, CV_8UC1, cv::Scalar(0));
    cv::Mat image_gray_equalization(height, width, CV_8UC1, cv::Scalar(0));

    while(1){
        cap >> image;

        cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);

        histogram_equaliation(image_gray, &image_gray_equalization);

        cv::calcHist(&image_gray, 1, 0, cv::Mat(), histGray, 1,
                     &nbins, &histrange,
                     uniform, acummulate);
        cv::calcHist(&image_gray_equalization, 1, 0, cv::Mat(), histGrayEq, 1,
                     &nbins, &histrange,
                     uniform, acummulate);

        cv::normalize(histGray, histGray, 0, histImgGray.rows, cv::NORM_MINMAX, -1, cv::Mat());
        cv::normalize(histGrayEq, histGrayEq, 0, histImgGrayEq.rows, cv::NORM_MINMAX, -1, cv::Mat());
        
        histImgGray.setTo(cv::Scalar(0));
        histImgGrayEq.setTo(cv::Scalar(0));
        
        for(int i=0; i<nbins; i++){
            cv::line(histImgGray,
                cv::Point(i, histh),
                cv::Point(i, histh-cvRound(histGray.at<float>(i))),
                cv::Scalar(255), 1, 8, 0);
            cv::line(histImgGrayEq,
                cv::Point(i, histh),
                cv::Point(i, histh-cvRound(histGrayEq.at<float>(i))),
                cv::Scalar(255), 1, 8, 0);
        }

        histImgGray.copyTo(image_gray(cv::Rect(0, 0,nbins, histh)));
        histImgGrayEq.copyTo(image_gray_equalization(cv::Rect(0, 0,nbins, histh)));

        cv::imshow("Image Normal", image_gray);
        cv::imshow("Equalizado", image_gray_equalization);

        key = cv::waitKey(30);
        if(key == 27) break;
    }
    return 0;
}