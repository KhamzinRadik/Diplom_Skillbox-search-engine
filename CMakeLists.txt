cmake_minimum_required(VERSION 3.28)

set(unit_name "search_engine")
project(${unit_name} CXX)

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
include(FetchContent)
include(FetchContent)

# Find and include nlohmann_json using FetchContent (assuming required)
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(nlohmann_json)

# Find and include gtest using FetchContent (assuming required)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# Find and include spdlog using FetchContent (assuming required)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
)

FetchContent_MakeAvailable(spdlog)

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
add_executable(${unit_name} ${CMAKE_CURRENT_SOURCE_DIR}/${unit_name}/main.cpp ${sources} ${headers})
target_link_libraries(${unit_name} PRIVATE nlohmann_json::nlohmann_json spdlog)

# Now simply link against gtest or gtest_main as needed. Eg
add_test(NAME example_test COMMAND ${unit_name})