#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>


#include "../include/ShapeFunction.hpp"

namespace py = pybind11;


PYBIND11_MODULE(shape_module, m) {
    m.doc() = "Python Module for ShapeFunction class";


    py::class_<ShapeFunction>(m, "ShapeFunction")

        .def(py::init<int>(), py::arg("p"))
        
        .def("compute_phi", &ShapeFunction::compute_phi, py::arg("ksi"), py::arg("idx"))
        .def("compute_dphi", &ShapeFunction::compute_dphi, py::arg("ksi"), py::arg("idx"))
        .def("set_cache_val", &ShapeFunction::set_cache_val, py::arg("integration_points"))
        .def("get_cached_phi", &ShapeFunction::get_cached_phi, py::arg("point_idx"), py::arg("sh_nb"))
        .def("get_cached_dphi", &ShapeFunction::get_cached_dphi, py::arg("point_idx"), py::arg("sh_nb")) 
        .def("get_p", &ShapeFunction::get_p)
        .def("save_shape_functions", &ShapeFunction::save_shape_functions, py::arg("ksi_vec"), py::arg("outdir"));
}