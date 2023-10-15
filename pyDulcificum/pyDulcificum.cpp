#include <dulcificum/miracle_jtp/mgjtp_command_to_json.h>
#include <dulcificum/miracle_jtp/mgjtp_json_to_command.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#ifndef PYDULCIFICUM_VERSION
#define PYDULCIFICUM_VERSION "0.1.0"
#endif

namespace py = pybind11;

PYBIND11_MODULE(pyDulcificum, module)
{
    module.doc() = R"pbdoc(exit
        pyDulcificum
        -----------------------
        .. currentmodule:: Python bindings for Dulcificum
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";
    module.attr("__version__") = PYDULCIFICUM_VERSION;

    py::class_<dulcificum::botcmd::Move>(module, "Move")
        .def(py::init<>())
        .def_readwrite("point", &dulcificum::botcmd::Move::point, "The point of the move command")
        .def_readwrite("feedrate", &dulcificum::botcmd::Move::feedrate, "The feedrate of the move command")
        .def_readwrite("is_point_relative", &dulcificum::botcmd::Move::is_point_relative, "Components of the point relative")
        ;
}