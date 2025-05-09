#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <opencv2/opencv.hpp>
#include "PIJCodec.hpp"

namespace py = pybind11;

namespace pybind11 { namespace detail {
    template <> struct type_caster<cv::Mat> {
    public:
        PYBIND11_TYPE_CASTER(cv::Mat, _("numpy.ndarray"));

        bool load(handle src, bool) {
            py::array_t<unsigned char> array = py::array_t<unsigned char>::ensure(src);
            if (!array) return false;

            auto buf = array.request();
            if (buf.ndim != 3) return false;

            value = cv::Mat(
                buf.shape[0], 
                buf.shape[1], 
                CV_8UC3, 
                (unsigned char*)buf.ptr
            ).clone();
            return true;
        }

        static handle cast(const cv::Mat& mat, return_value_policy, handle) {
            std::vector<ssize_t> shape = {mat.rows, mat.cols, 3};
            std::vector<ssize_t> strides = {
                static_cast<ssize_t>(mat.step[0]), 
                static_cast<ssize_t>(mat.step[1]), 
                static_cast<ssize_t>(sizeof(unsigned char))
            };
            
            return py::array_t<unsigned char>(
                shape,          // shape
                strides,       // strides
                mat.data,      // data pointer
                py::none()     // parent object
            ).release();
        }
    };
}} // namespace pybind11::detail

py::bytes pij_encode(const cv::Mat& img,
    const std::vector<std::vector<int>>& boxes,
    int pad = 5, int jpeg_q = 85, int zlib_lvl = 6)
{
    std::ostringstream os(std::ios::binary);
    PIJCodec::encode(img, boxes, os, pad, jpeg_q, zlib_lvl);
    return py::bytes(os.str());
}

cv::Mat pij_decode(const py::bytes& blob)
{
    std::string buf = blob;
    std::istringstream is(buf, std::ios::binary);
    return PIJCodec::decode(is);
}

PYBIND11_MODULE(pypij, m) {
    m.doc() = "Python bindings for the PIJ (PNG-inside-JPEG) codec";
    m.def("encode", &pij_encode,
        py::arg("image"), py::arg("lossless_boxes"),
        py::arg("pad") = 5, py::arg("jpeg_quality") = 85,
        py::arg("zlib_level") = 6);
        m.def("decode", &pij_decode, py::arg("blob"));
}