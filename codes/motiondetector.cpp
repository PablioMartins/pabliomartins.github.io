#include <iostream>
#include <opencv2/opencv.hpp>

bool move_detection(cv::Mat hist_current, cv::Mat hist_old, int limiar){
    double erro = 0;
    
    for(int i=0; i < 256; i++){
        if(hist_current.at<u_int8_t>(i) != 0)
            erro += abs((hist_current.at<u_int8_t>(i) - hist_old.at<u_int8_t>(i))/double(hist_current.at<u_int8_t>(i)));
    }
    erro = erro/256.0;
    if(erro > limiar)
        return true;
    else 
        return false;
}

int main(int argc, char **argv){
    cv::Mat image;
    int width, height;
    cv::VideoCapture cap;
    std::vector<cv::Mat> planes;
    cv::Mat histR, histR_old, histG, histB;
    int nbins = 64;
    float range[] = {0, 255};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    int key;
    int limiar = 2.0;

    cap.open(0);

    if(!cap.isOpened()){
        std::cout << "Camera indisponivel";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins/2;
    cv::Mat histImgR(histh, histw, CV_8UC3, cv::Scalar(0,0,0));
    cv::Mat histImgG(histh, histw, CV_8UC3, cv::Scalar(0,0,0));
    cv::Mat histImgB(histh, histw, CV_8UC3, cv::Scalar(0,0,0));

    while (1){
        cap >> image;
        cv::split (image, planes);
        cv::calcHist(&planes[0], 1, 0, cv::Mat(), histB, 1,
                    &nbins, &histrange,
                    uniform, acummulate);
        cv::calcHist(&planes[1], 1, 0, cv::Mat(), histG, 1,
                    &nbins, &histrange,
                    uniform, acummulate);
        cv::calcHist(&planes[2], 1, 0, cv::Mat(), histR, 1,
                    &nbins, &histrange,
                    uniform, acummulate);

        cv::normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());
        cv::normalize(histG, histG, 0, histImgG.rows, cv::NORM_MINMAX, -1, cv::Mat());
        cv::normalize(histB, histB, 0, histImgB.rows, cv::NORM_MINMAX, -1, cv::Mat());

        histImgR.setTo(cv::Scalar(0));
        histImgG.setTo(cv::Scalar(0));
        histImgB.setTo(cv::Scalar(0));

        for(int i=0; i<nbins; i++){
            cv::line(histImgR,
                cv::Point(i, histh),
                cv::Point(i, histh-cvRound(histR.at<float>(i))),
                cv::Scalar(0, 0, 255), 1, 8, 0);
            cv::line(histImgG,
                cv::Point(i, histh),
                cv::Point(i, histh-cvRound(histG.at<float>(i))),
                cv::Scalar(0, 255, 0), 1, 8, 0);
            cv::line(histImgB,
                cv::Point(i, histh),
                cv::Point(i, histh-cvRound(histB.at<float>(i))),
                cv::Scalar(255, 0, 0), 1, 8, 0);
        }

        histImgR.copyTo(image(cv::Rect(0, 0       ,nbins, histh)));
        histImgG.copyTo(image(cv::Rect(0, histh   ,nbins, histh)));
        histImgB.copyTo(image(cv::Rect(0, 2*histh ,nbins, histh)));

        if(!histR_old.empty() && move_detection(histR, histR_old, limiar)){
            cv::putText(image, "Move Detect", cv::Point2d(height/2, width/2), cv::FONT_HERSHEY_SIMPLEX, 1,cv::Scalar(0,0,255));
        }
        
        cv::imshow("image", image);
       
        histR.copyTo(histR_old);
        
        key = cv::waitKey(30);
        if(key == 27) break;
    }
    return 0;
}