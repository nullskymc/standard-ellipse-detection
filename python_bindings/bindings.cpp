#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "detect.h"
#include "ellipse_detection/types.hpp" // 添加这行，因为Point类定义在这里

namespace py = pybind11;
py::list detectEllipse_wrapper(py::array_t<uint8_t> image, int row, int col,
                               int polarity, double width) {
    // 获取 NumPy 数组的缓冲信息
    py::buffer_info bufInfo = image.request();

    // 检查输入是否为二维数组，并且数据类型为 uint8
    if (bufInfo.ndim != 2) {
        throw std::runtime_error("图像数据必须是二维数组。");
    }
    if (bufInfo.format != py::format_descriptor<uint8_t>::format()) {
        throw std::runtime_error("图像数据类型必须为 uint8。");
    }

    // 获取指向图像数据的指针
    uint8_t* imageData = static_cast<uint8_t*>(bufInfo.ptr);

    // 创建 Ellipse 容器
    std::vector<std::shared_ptr<zgh::Ellipse>> ells;

    // 调用实际的 detectEllipse 函数
    zgh::detectEllipse(imageData, row, col, ells, polarity, width);

    // 将检测到的椭圆信息返回给 Python
    py::list result;
    for (const auto& ell : ells) {
        py::dict ell_dict;
        ell_dict["o"] = py::make_tuple(ell->o.x, ell->o.y);
        ell_dict["a"] = ell->a;
        ell_dict["b"] = ell->b;
        ell_dict["phi"] = ell->phi;
        ell_dict["polarity"] = ell->polarity;
        ell_dict["goodness"] = ell->goodness;
        ell_dict["coverangle"] = ell->coverangle;
        ell_dict["inliers"] = py::list();
        for (const auto& inlier : ell->inliers) {
            ell_dict["inliers"].cast<py::list>().append(py::make_tuple(inlier.x, inlier.y));
        }
        result.append(ell_dict);
    }

    return result;
}


PYBIND11_MODULE(detect_module, m) {
    // Point类改为使用Pointd (double类型的Point)
    py::class_<zgh::Pointd>(m, "Point")
        .def(py::init<>())
        .def(py::init<double, double>())
        .def_readwrite("x", &zgh::Pointd::x)
        .def_readwrite("y", &zgh::Pointd::y);

    // 绑定 Ellipse 类
    py::class_<zgh::Ellipse, std::shared_ptr<zgh::Ellipse>>(m, "Ellipse")
        .def(py::init<>())
        .def_readwrite("o", &zgh::Ellipse::o)
        .def_readwrite("a", &zgh::Ellipse::a)
        .def_readwrite("b", &zgh::Ellipse::b)
        .def_readwrite("phi", &zgh::Ellipse::phi)
        .def_readwrite("polarity", &zgh::Ellipse::polarity)
        .def_readwrite("goodness", &zgh::Ellipse::goodness)
        .def_readwrite("coverangle", &zgh::Ellipse::coverangle)
        .def_readwrite("inliers", &zgh::Ellipse::inliers);

    // 绑定 detectEllipse 函数
    m.def("detectEllipse", &detectEllipse_wrapper,
          py::arg("image"),
          py::arg("row"),
          py::arg("col"),
          py::arg("polarity"),
          py::arg("width"),
          "调用底层 C++ detectEllipse 函数的包装");
}