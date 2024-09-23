cmake_minimum_required(VERSION 3.28)

set(unit_name "search_engine")
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD 20)
project(${unit_name} CXX)

set(TEST ON)

# Check if unit_name is defined
if (NOT DEFINED unit_name)
    message(FATAL_ERROR "unit_name is not defined! Please set it before including this CMakeLists.txt.")
endif ()


if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/configs/configs.json")
    message(FATAL_ERROR "Файл configs.json не найден! Он обязателен для сборки.")
endif ()

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/configs/request.json")
    message(FATAL_ERROR "Файл request.json не найден! Он обязателен для сборки.")
endif ()


include(FetchContent)


# Find and include nlohmann_json using FetchContent (assuming required)
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

# Find and include spdlog using FetchContent (assuming required)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
)

# Find source and header files using GLOB with informative message
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/${unit_name}/*.cpp)
message(STATUS "Found source files:")
foreach (source IN LISTS sources)
    message(STATUS "  - ${source}")
endforeach (source)

file(GLOB_RECURSE headers
        ${PROJECT_SOURCE_DIR}/include/${unit_name}/utils/*.h
        ${PROJECT_SOURCE_DIR}/include/${unit_name}/*.h
)
message(STATUS "Found header files:")
foreach (header IN LISTS headers)
    message(STATUS "  - ${header}")
endforeach (header)

include_directories(${unit_name}
        include/search_engine
)

FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(spdlog)

add_executable(${unit_name} ${CMAKE_CURRENT_SOURCE_DIR}/${unit_name}/main.cpp ${sources} ${headers})
target_link_libraries(${unit_name} PRIVATE nlohmann_json::nlohmann_json spdlog)


if (TEST)
    set(unit_test "run-test")

    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS} include/${unit_name})

    # Create the test executable
    add_executable(${unit_test}
            tests/test_main.cpp
            tests/test_InvertedIndex.cpp
            tests/test_SearchServer.cpp

            include/${unit_name}/JsonFill.h
            include/${unit_name}/ConverterJson.h
            include/${unit_name}/InvertedIndex.h
            include/${unit_name}/SearchServer.h

            ${unit_name}/JsonFill.cpp
            ${unit_name}/InvertedIndex.cpp
            ${unit_name}/ConvertedJson.cpp
            ${unit_name}/SearchServer.cpp
    )
    target_link_libraries(${unit_test} ${GTEST_LIBRARIES} pthread nlohmann_json::nlohmann_json spdlog)
endif (TEST)



