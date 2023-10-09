#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char**argv) {
    cv::Mat imagemEscondida, imagemCodificada;
    cv::Vec3b valEscondida, valCodificada;

    imagemCodificada = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (imagemCodificada.empty()) {
        std::cout << "imagem nao carregou corretamente" << std::endl;
        return (-1);
    }

    imagemEscondida = cv::Mat::zeros(imagemCodificada.rows, imagemCodificada.cols, CV_8UC3);

    for (int i = 0; i < imagemCodificada.rows; i++) {
        for (int j = 0; j < imagemCodificada.cols; j++) {
            valCodificada = imagemCodificada.at<cv::Vec3b>(i, j);
            valEscondida[0] = (0b00000111 & valCodificada[0]) << 5;
            valEscondida[1] = (0b00000111 & valCodificada[1]) << 5;
            valEscondida[2] = (0b00000111 & valCodificada[2]) << 5;
            imagemEscondida.at<cv::Vec3b>(i, j) = valEscondida;
        }
    }
    imwrite("./resultimg/esteganografia.png", imagemEscondida);
    return 0;
}