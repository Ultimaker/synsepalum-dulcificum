import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.env import VirtualBuildEnv
from conan.tools.files import copy, mkdir, AutoPackager, update_conandata
from conan.tools.microsoft import check_min_vs, is_msvc_static_runtime, is_msvc
from conan.tools.scm import Version, Git
from conan.errors import ConanException


required_conan_version = ">=1.58.0 <2.0.0"


class DulcificumConan(ConanFile):
    name = "dulcificum"
    description = "Dulcificum changes the flavor, or dialect, of 3d printer commands"
    author = "UltiMaker"
    license = ""
    url = "https://github.com/Ultimaker/synsepalum-dulcificum"
    homepage = "https://ultimaker.com"
    topics = ("cura", "curaengine", "gcode-generation", "3D-printing", "miraclegrue", "toolpath")
    package_type = "library"
    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_extensive_warnings": [True, False],
        "with_apps": [True, False],
        "with_python_bindings": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_extensive_warnings": False,
        "with_apps": False,
        "with_python_bindings": True,
    }

    def set_version(self):
        if not self.version:
            self.version = self.conan_data["version"]

    @property
    def _min_cppstd(self):
        return 20

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "11",
            "clang": "14",
            "apple-clang": "13",
            "msvc": "192",
            "visual_studio": "17",
        }

    @property
    def _run_tests(self):
        if self.settings.compiler == "apple-clang" and Version(self.settings.compiler.version) <= Version("14"):
            return False
        return not self.conf.get("tools.build:skip_test", False, check_type = bool)

    def export(self):
        try:
            git = Git(self)
            commit_hash = git.get_commit()
        except ConanException as e:
            commit_hash = "unknown"
            self.output.error(f"An error occurred: {e}")
        update_conandata(self, {"version": self.version, "commit_hash": commit_hash})

    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
        copy(self, "*", os.path.join(self.recipe_folder, "src"), os.path.join(self.export_sources_folder, "src"))
        copy(self, "*", os.path.join(self.recipe_folder, "include"), os.path.join(self.export_sources_folder, "include"))
        copy(self, "*", os.path.join(self.recipe_folder, "test"), os.path.join(self.export_sources_folder, "test"))
        copy(self, "*", os.path.join(self.recipe_folder, "apps"), os.path.join(self.export_sources_folder, "apps"))
        copy(self, "*", os.path.join(self.recipe_folder, "pyDulcificum"), os.path.join(self.export_sources_folder, "pyDulcificum"))
        copy(self, "*", os.path.join(self.recipe_folder, "DulcificumJS"), os.path.join(self.export_sources_folder, "DulcificumJS"))

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
        if self.settings.arch == "wasm" and self.settings.os == "Emscripten":
            del self.options.with_python_bindings

    def configure(self):
        if self.options.get_safe("with_python_bindings", False):
            self.options["cpython"].shared = True

    def layout(self):
        cmake_layout(self)

        self.cpp.build.bin = ["translator"]
        self.cpp.build.bindirs = ["apps"]

        self.cpp.package.lib = ["dulcificum", "pyDulcificum"]
        self.cpp.package.libdirs = ["lib"]
        self.cpp.package.bindirs = ["bin"]

    def requirements(self):
        self.requires("nlohmann_json/3.11.2", transitive_headers = True)
        self.requires("range-v3/0.12.0")
        self.requires("spdlog/1.12.0")
        self.requires("fmt/10.1.1")
        self.requires("ctre/3.7.2")
        if self.options.with_apps:
            self.requires("docopt.cpp/0.6.3")
        if self.options.get_safe("with_python_bindings", False):
            self.requires("cpython/3.10.4@ultimaker/stable")
            self.requires("pybind11/2.10.4")

    def build_requirements(self):
        self.test_requires("standardprojectsettings/[>=0.1.0]@ultimaker/stable")
        if self._run_tests:
            self.test_requires("gtest/[>=1.12.1]")

    def validate(self):
        if self.settings.compiler.cppstd:
            check_min_cppstd(self, self._min_cppstd)
        check_min_vs(self, 191)
        if not is_msvc(self):
            minimum_version = self._compilers_minimum_version.get(str(self.settings.compiler), False)
            if minimum_version and Version(self.settings.compiler.version) < minimum_version:
                raise ConanInvalidConfiguration(
                    f"{self.ref} requires C++{self._min_cppstd}, which your compiler does not support."
                )
        if is_msvc(self) and self.options.shared:
            raise ConanInvalidConfiguration(f"{self.ref} can not be built as shared on Visual Studio and msvc.")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["ENABLE_TESTS"] = self._run_tests
        tc.variables["EXTENSIVE_WARNINGS"] = self.options.enable_extensive_warnings
        tc.variables["DULCIFICUM_VERSION"] = self.version
        tc.variables["GIT_COMMIT_HASH"] = self.conan_data["commit_hash"]
        tc.variables["WITH_APPS"] = self.options.with_apps
        if self.options.with_apps:
            tc.variables["APP_VERSION"] = self.version

        if self.settings.arch == "wasm" and self.settings.os == "Emscripten":
            tc.variables["WITH_JS_BINDINGS"] = True
        else:
            tc.variables["WITH_JS_BINDINGS"] = False
        tc.variables["WITH_PYTHON_BINDINGS"] = self.options.get_safe("with_python_bindings", False)
        if self.options.get_safe("with_python_bindings", False):
            tc.variables["PYTHON_EXECUTABLE"] = self.deps_user_info["cpython"].python.replace("\\", "/")
            tc.variables["Python_USE_STATIC_LIBS"] = not self.options["cpython"].shared
            tc.variables["Python_ROOT_DIR"] = self.deps_cpp_info["cpython"].rootpath.replace("\\", "/")
            tc.variables["Python_FIND_FRAMEWORK"] = "NEVER"
            tc.variables["Python_FIND_REGISTRY"] = "NEVER"
            tc.variables["Python_FIND_IMPLEMENTATIONS"] = "CPython"
            tc.variables["Python_FIND_STRATEGY"] = "LOCATION"
            tc.variables["PYDULCIFICUM_VERSION"] = self.version

        if is_msvc(self):
            tc.variables["USE_MSVC_RUNTIME_LIBRARY_DLL"] = not is_msvc_static_runtime(self)
        tc.cache_variables["CMAKE_POLICY_DEFAULT_CMP0077"] = "NEW"
        tc.generate()

        tc = CMakeDeps(self)
        tc.generate()

        tc = VirtualBuildEnv(self)
        tc.generate(scope = "build")

        for dep in self.dependencies.values():
            if len(dep.cpp_info.libdirs) > 0:
                copy(self, "*.dylib", dep.cpp_info.libdirs[0], self.build_folder)
                copy(self, "*.dll", dep.cpp_info.libdirs[0], self.build_folder)
            if len(dep.cpp_info.bindirs) > 0:
                copy(self, "*.dll", dep.cpp_info.bindirs[0], self.build_folder)
            if self._run_tests:
                test_path = os.path.join(self.build_folder,  "tests")
                if not os.path.exists(test_path):
                    mkdir(self, test_path)
                if len(dep.cpp_info.libdirs) > 0:
                    copy(self, "*.dylib", dep.cpp_info.libdirs[0], os.path.join(self.build_folder,  "tests"))
                    copy(self, "*.dll", dep.cpp_info.libdirs[0], os.path.join(self.build_folder,  "tests"))
                if len(dep.cpp_info.bindirs) > 0:
                    copy(self, "*.dll", dep.cpp_info.bindirs[0], os.path.join(self.build_folder,  "tests"))


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    def deploy(self):
        copy(self, "dulcificum_js*", src=os.path.join(self.package_folder, "lib"), dst=self.install_folder)

    def package(self):
        copy(self, pattern="LICENSE", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        copy(self, "translator*", src = os.path.join(self.build_folder, "apps"), dst = os.path.join(self.package_folder, "bin"), keep_path = False)
        copy(self, "*.pyd", src = os.path.join(self.build_folder, "pyDulcificum"), dst = os.path.join(self.package_folder, "lib", "pyDulcificum"), keep_path = False)
        copy(self, pattern="dulcificum_js.*", src=os.path.join(self.build_folder, "DulcificumJS", "cpp"), dst=os.path.join(self.package_folder, "lib"))
        copy(self, f"*.d.ts", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path = False)
        copy(self, f"*.js", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path = False)
        packager = AutoPackager(self)
        packager.run()

    def package_info(self):
        if self.in_local_cache:
            self.runenv_info.append_path("PYTHONPATH", os.path.join(self.package_folder, "lib", "pyDulcificum"))
        else:
            self.runenv_info.append_path("PYTHONPATH", os.path.join(self.build_folder, "pyDulcificum"))
