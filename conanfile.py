import os
from conans import ConanFile, CMake


class DiscreteGeoKernelConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "gtest/1.12.1",
        "nlohmann_json/3.11.2",
    ]

    generators = "cmake"
    options = {}
    default_options = {}

    def build(self):
        cmake = CMake(self)
        cmake.verbose()
        cmake.configure()
        cmake.build()

    def requirements(self):
        pass

    def imports(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        if "cmake_multi" in self.generators:
            self.copy("*.dll", dst="bin/" + str(self.settings.build_type), keep_path=False)
        else:
            self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so*", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
