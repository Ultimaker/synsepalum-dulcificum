#include <dulcificum/utils/io.h>
#include <filesystem>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>

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

    module.def(
        "read_file",
        [](const std::string& path)
        {
            return dulcificum::utils::readFile(std::filesystem::path(path));
        },
        py::arg("filename"),
        "Reads the content of a file and returns it as a string.");

    module.def(
        "write_file",
        [](const std::string& path, std::string& content)
        {
            return dulcificum::utils::writeFile(std::filesystem::path(path), content);
        },
        py::arg("filename"),
        py::arg("content"),
        "Writes a given string to a file.");
}