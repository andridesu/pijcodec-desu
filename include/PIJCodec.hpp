#pragma once

#include <vector>
#include <istream>
#include <opencv2/opencv.hpp>

class PIJCodec {
public:
    // Encode the input image, writing .pij data to the output stream
    static void encode(
        const cv::Mat& input,
        const std::vector<std::vector<int>>& losslessBoxes,
        std::ostream& out,
        int pad = 5,
        int jpegQuality = 85,
        int zlibLevel = 6
    );

    // Decode from .pij stream, returning a reconstructed cv::Mat
    static cv::Mat decode(std::istream& in);
};