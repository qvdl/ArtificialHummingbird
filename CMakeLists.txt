cmake_minimum_required(VERSION 3.16)
project(SF2565_PROJECT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        taskflow
        GIT_REPOSITORY https://github.com/taskflow/taskflow.git
        GIT_TAG v3.7.0
)
FetchContent_MakeAvailable(taskflow)

add_executable(SF2565_PROJECT main.cpp
        AHA_Optimizer.cpp
        AHA_Optimizer.h
        VisitTable.cpp
        VisitTable.h
        Hummingbird.h
        FlightEngine.cpp
        FlightEngine.h
        AHA_Config.h
        Problem.h
        RNG_Utils.h
        RNG_Utils.cpp
        Problems/Sphere.h
        Problems/AbsoluteValue.h
        Problems/ShiftedQuadratic.h
        helpers.cpp
        helpers.h
        Problems/Rastrigin.h
        Problems/Rosenbrock.h
        Problems/Schwefel12.h
        Problems/Schwefel226.h
        Problems/Griewank.h
)

target_link_libraries(SF2565_PROJECT PRIVATE Taskflow)
target_compile_definitions(SF2565_PROJECT PRIVATE AHA_WITH_TASKFLOW)