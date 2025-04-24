#include "PIJCodec.hpp"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: pij_viewer <file.pij>\n";
        return 1;
    }
    std::ifstream f(argv[1], std::ios::binary);
    if (!f) { std::cerr << "Cannot open " << argv[1]; return 1; }
    try {
        cv::Mat img = PIJCodec::decode(f);
        cv::imshow("PIJ Viewer", img);
        cv::waitKey(0);
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}