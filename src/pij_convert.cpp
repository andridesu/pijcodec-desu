#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include "PIJCodec.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: pij_convert <input.pij> <output.[jpg|png]>\n";
        return 1;
    }

    const std::string input_filename = argv[1];
    const std::string output_filename = argv[2];

    // Validate input file extension
    if (input_filename.substr(input_filename.find_last_of(".") + 1) != "pij") {
        std::cerr << "Error: input file must be a .pij file\n";
        return 1;
    }

    // Validate output file extension
    std::string output_ext = output_filename.substr(output_filename.find_last_of(".") + 1);
    if (output_ext != "jpg" && output_ext != "png") {
        std::cerr << "Error: output format must be either .jpg or .png\n";
        return 1;
    }

    // Open input file
    std::ifstream in(input_filename, std::ios::binary);
    if (!in) {
        std::cerr << "Error: cannot open input file `" << input_filename << "`\n";
        return 1;
    }

    try {
        // Decode PIJ image
        cv::Mat img = PIJCodec::decode(in);
        if (img.empty()) {
            std::cerr << "Error: decoded image is empty\n";
            return 1;
        }

        // Convert BGR to RGB if needed
        if (img.channels() == 3) {  // Only for color images
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        }

        // Write output image
        std::vector<int> compression_params;
        if (output_ext == "jpg") {
            compression_params = {cv::IMWRITE_JPEG_QUALITY, 95};
        }

        if (!cv::imwrite(output_filename, img, compression_params)) {
            std::cerr << "Error: failed to write output image\n";
            return 1;
        }

        std::cout << "Successfully converted " << input_filename 
                  << " to " << output_filename << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Conversion failed: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}