#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "PIJCodec.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: pij_viewer <file.pij>\n";
        return 1;
    }

    const std::string filename = argv[1];
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Error: cannot open file `" << filename << "`\n";
        return 1;
    }

    try {
        cv::Mat img = PIJCodec::decode(in);
        if (img.empty()) {
            std::cerr << "Error: decoded image is empty\n";
            return 1;
        }

        // Convert BGR to RGB if needed
        if (img.channels() == 3) {  // Only for color images
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        }

        // Show and wait
        cv::namedWindow("PIJ Viewer", cv::WINDOW_AUTOSIZE);
        cv::imshow("PIJ Viewer", img);
        std::cout << "Press any key in the image window to exit...\n";
        cv::waitKey(0);
    }
    catch (const std::exception& ex) {
        std::cerr << "Decode failed: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
