option(USE_CONAN "Install dependencies with conan" ON)
macro(config_conan)
    # List of packages to install
    set(CONAN_DEPS
#        boost/1.77.0
        glad/0.1.34
        glm/0.9.9.8
        stb/20200203
        spdlog/1.9.2
    )

    # Configuration of packages
    set(CONAN_OPTIONS
    )

    # Path to place conan.cmake file
    set(CONAN_CMAKE_PATH ${CMAKE_BINARY_DIR}/conan.cmake)

    # Directory to place modules for installed packages
    set(CONAN_INSTALL_DIR ${CMAKE_BINARY_DIR}/vendor)
endmacro()

# Macro to run in main CMakeLists
macro(run_conan)
    if(USE_CONAN)
        config_conan()
        list(APPEND CMAKE_MODULE_PATH ${CONAN_INSTALL_DIR})
        list(APPEND CMAKE_PREFIX_PATH ${CONAN_INSTALL_DIR})
        if(NOT EXISTS ${CONAN_CMAKE_PATH})
            message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
            file(DOWNLOAD
                "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                ${CONAN_CMAKE_PATH}
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON
            )
        endif()
        include(${CONAN_CMAKE_PATH})

        # Setup configuration
        conan_cmake_configure(
            REQUIRES ${CONAN_DEPS}
            GENERATORS cmake
            OPTIONS ${CONAN_OPTIONS}
        )
        # Install libraries from conan with considering of build type
        foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
            conan_cmake_autodetect(settings BUILD_TYPE ${TYPE})
            conan_cmake_install(
                PATH_OR_REFERENCE .
                BUILD missing
                REMOTE conancenter
                SETTINGS ${settings}
                INSTALL_FOLDER ${CONAN_INSTALL_DIR}
            )
        endforeach()

        include(${CONAN_INSTALL_DIR}/conanbuildinfo.cmake)
        conan_basic_setup(TARGETS)
    endif()
endmacro()
