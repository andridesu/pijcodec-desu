#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <opencv2/opencv.hpp>
#include "PIJCodec.hpp"

namespace py = pybind11;

/* Helpers -------------------------------------------------------------- */
static cv::Mat numpy_to_mat(const py::array& arr) {
    if (arr.ndim() != 3 || arr.shape(2) != 3)
        throw std::runtime_error("Input must be H×W×3 uint8");
    return cv::Mat(
        static_cast<int>(arr.shape(0)),
        static_cast<int>(arr.shape(1)),
        CV_8UC3,
        const_cast<uint8_t*>(static_cast<const uint8_t*>(arr.data()))
    ).clone();                                // copy to own buffer
}

static py::array_t<uint8_t> mat_to_numpy(const cv::Mat& m) {
    // we want a H×W×3 uint8 array
    // step: bytes per row; each pixel is 3 bytes
    py::array_t<uint8_t> arr(
        { m.rows, m.cols, 3 },                                     // shape
        { static_cast<py::ssize_t>(m.step),     // bytes to next row
          static_cast<py::ssize_t>(3),           // bytes to next column
          static_cast<py::ssize_t>(1) },         // bytes to next channel
        m.data,                                  // pointer to data
        py::cast(m)                              // keep Mat alive
    );
    return arr;
}


/* Python-visible wrappers --------------------------------------------- */
py::bytes pij_encode(const py::array& img,
    const std::vector<std::vector<int>>& boxes,
    int pad = 5, int jpeg_q = 85, int zlib_lvl = 6)
{
    cv::Mat cv_img = numpy_to_mat(img);
    std::ostringstream os(std::ios::binary);
    PIJCodec::encode(cv_img, boxes, os, pad, jpeg_q, zlib_lvl);          // :contentReference[oaicite:0]{index=0}&#8203;:contentReference[oaicite:1]{index=1}
    return py::bytes(os.str());
}

py::array pij_decode(const py::bytes& blob)
{
    std::string buf = blob;
    std::istringstream is(buf, std::ios::binary);
    cv::Mat cv_img = PIJCodec::decode(is);                              // :contentReference[oaicite:2]{index=2}&#8203;:contentReference[oaicite:3]{index=3}
    return mat_to_numpy(cv_img);
}

/* Module definition ---------------------------------------------------- */
PYBIND11_MODULE(pijcodec, m) {
    m.doc() = "Python bindings for the PIJ (PNG-inside-JPEG) codec";
    m.def("encode", &pij_encode,
        py::arg("image"), py::arg("lossless_boxes"),
        py::arg("pad") = 5, py::arg("jpeg_quality") = 85,
        py::arg("zlib_level") = 6);
    m.def("decode", &pij_decode, py::arg("blob"));
}
