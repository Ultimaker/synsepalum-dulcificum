import shutil
import os

from io import StringIO
from pathlib import Path

from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.env import VirtualRunEnv
from conan.errors import ConanException
from conan.tools.files import copy


class DulcificumTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators =  "VirtualRunEnv"
    test_type = "explicit"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def generate(self):
        venv = VirtualRunEnv(self)
        venv.generate()

        for dep in self.dependencies.values():
            if len(dep.cpp_info.libdirs) > 0:
                copy(self, "*.dylib", dep.cpp_info.libdirs[0], self.build_folder)
                copy(self, "*.dll", dep.cpp_info.libdirs[0], self.build_folder)
                copy(self, "*.pyd", dep.cpp_info.libdirs[0], self.build_folder)
            if len(dep.cpp_info.bindirs) > 0:
                copy(self, "*.dll", dep.cpp_info.bindirs[0], self.build_folder)

    def build(self):
        if can_run(self):
            shutil.copy(Path(self.source_folder).joinpath("test.py"), Path(self.build_folder).joinpath("test.py"))

    def test(self):
        if can_run(self):
            test_buf = StringIO()
            self.run("python test.py", env = "conanrun", output = test_buf, run_environment = True)
            ret_val = test_buf.getvalue()
            if "gcode_2_miracle_jtp" not in ret_val:
                raise ConanException("pyDulcificum wasn't build correctly!")