option(USE_CONAN "Install dependencies with conan" ON)
macro(config_conan)
    # List of packages to install
    option(QT_FROM_CONAN "Install Qt as dependency from conan" ON)
    set(CONAN_DEPS
        # boost/1.77.0
        glad/0.1.34
        glm/0.9.9.8
        stb/20200203
        spdlog/1.9.2
        magic_enum/0.7.3
    )
    if(QT_FROM_CONAN)
        list(APPEND CONAN_DEPS
            qt/6.2.2
        )
    endif()
    if(UNIX)
        list(APPEND CONAN_DEPS 
            expat/2.4.2
        )
    endif()

    # Configuration of packages
    set(CONAN_OPTIONS
        glad:gl_version=4.4
        glad:gl_profile=core
    )
    if(QT_FROM_CONAN)
        list(APPEND CONAN_OPTIONS
            qt:shared=True
        )
    endif()

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
                "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake"
                ${CONAN_CMAKE_PATH}
                EXPECTED_HASH SHA256=3bef79da16c2e031dc429e1dac87a08b9226418b300ce004cc125a82687baeef
                TLS_VERIFY ON
            )
        endif()  # NOT EXISTS ${CONAN_CMAKE_PATH}
        include(${CONAN_CMAKE_PATH})

        # Set import variables
        if(WIN32)
            STRING(CONCAT IMPORT_DLL_FILES "bin, *.dll -> " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
            STRING(CONCAT IMPORT_RES_DLL_FILES "res/archdatadir/plugins, * -> " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        endif()  # WIN32

        # Setup configuration
        conan_cmake_configure(
            REQUIRES ${CONAN_DEPS}
            GENERATORS cmake_find_package
            OPTIONS ${CONAN_OPTIONS}
            IMPORTS ${IMPORT_DLL_FILES}
            IMPORTS ${IMPORT_RES_DLL_FILES}
        )
        # Install libraries from conan with considering of build type
        get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
        if(isMultiConfig)
            foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
                conan_cmake_autodetect(settings BUILD_TYPE ${TYPE})
                conan_cmake_install(
                    PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings}
                    INSTALL_FOLDER ${CONAN_INSTALL_DIR}/${TYPE}
                )
            endforeach() # TYPE
        else()  # isMultiConfig
            conan_cmake_autodetect(settings BUILD_TYPE ${CMAKE_BUILD_TYPE})
            conan_cmake_install(
                PATH_OR_REFERENCE .
                BUILD missing
                REMOTE conancenter
                SETTINGS ${settings}
                INSTALL_FOLDER ${CONAN_INSTALL_DIR}
            )
        endif()  # isMultiConfig
    endif()  # USE_CONAN
endmacro()  # run_conan
