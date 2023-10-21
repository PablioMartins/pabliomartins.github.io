#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

int image_height, image_width;
float point_down = 0, point_up = 0, decay = 0, vertical_position = 0;

int height_slider = 0;
int height_slider_max = 100;

int decay_slider = 0;
int decay_slider_max = 100;

int vertical_position_slider = 0;
int vertical_position_slider_max = 100;

cv::Mat image, image_blurred, result;

char TrackbarName[50];

void show_image(){
  cv::Mat weighting_mask(image_height, image_width, CV_32FC3);
  cv::Mat weighting_mask_inverse(image_height, image_width, CV_32FC3);
  float line_value;
  float line_value_inverse;
  
  for(int i = 0; i < image_height; i++){

    if (decay != 0){
      line_value = 0.5*(tanh((i + vertical_position - point_down)/decay) - tanh((i + vertical_position - point_up)/decay));
      line_value_inverse = 1.0 - line_value;
    }
    else { 
      line_value = 1.0;
      line_value_inverse = 0.0;
    }

    for(int j = 0; j < image_width; j++){
      weighting_mask.at<cv::Vec3f>(i, j) = cv::Vec3f(line_value, line_value, line_value);
      weighting_mask_inverse.at<cv::Vec3f>(i, j) = cv::Vec3f(line_value_inverse, line_value_inverse, line_value_inverse);
    }
    
  }
  
  cv::Mat result_normal, result_inverse;

  cv::multiply(image, weighting_mask, result_normal);
  cv::multiply(image_blurred, weighting_mask_inverse, result_inverse);
  
  cv::addWeighted(result_normal, 1.0, result_inverse, 1.0, 0.0, result);

  result.convertTo(result, CV_8UC3);
  
  cv::imshow("Tiltshift", result);
}

void on_trackbar_height(int, void*){
  int height = image_height * height_slider / 100;
  point_down = -height/2;
  point_up = height/2;
  show_image();
}

void on_trackbar_decay(int, void*){
  decay = decay_slider;
  show_image();
}

void on_trackbar_position(int, void*){
  vertical_position = -vertical_position_slider * image_height / 100;
  show_image();
}

int main(int argvc, char** argv){
  image = cv::imread(argv[1]);

  image.convertTo(image, CV_32FC3);

  image_height = image.rows;
  image_width = image.cols;

  cv::GaussianBlur(image, image_blurred, cv::Size2i(17,17), 0);

  cv::namedWindow("Tiltshift", 1);
  
  std::sprintf( TrackbarName, "Posicao x %d", vertical_position_slider_max );
  cv::createTrackbar( TrackbarName, "Tiltshift",
                      &vertical_position_slider,
                      vertical_position_slider_max,
                      on_trackbar_position);
  on_trackbar_position(vertical_position_slider, 0 );
  
  std::sprintf( TrackbarName, "Largura x %d", height_slider_max );
  cv::createTrackbar( TrackbarName, "Tiltshift",
                      &height_slider,
                      height_slider_max,
                      on_trackbar_height );
  on_trackbar_height(height_slider, 0 );

  std::sprintf( TrackbarName, "Decaimento x %d", decay_slider_max );
  cv::createTrackbar( TrackbarName, "Tiltshift",
                      &decay_slider,
                      decay_slider_max,
                      on_trackbar_decay );
  on_trackbar_decay(decay_slider, 0 );

  cv::waitKey(0);
  return 0;
}