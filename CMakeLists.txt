cmake_minimum_required(VERSION 4.0)
project(SF2565_PROJECT)

set(CMAKE_CXX_STANDARD 20)

add_executable(SF2565_PROJECT main.cpp
        AHA_Optimizer.cpp
        AHA_Optimizer.h
        VisitTable.cpp
        VisitTable.h
        Hummingbird.h
        FlightEngine.cpp
        FlightEngine.h
        AHA_Config.h
        Problem.cpp
        Problem.h
        Benchmarks.cpp
        Benchmarks.h
        RNG_Utils.h
        RNG_Utils.cpp
        Problems/SphereProblem.h
        Problems/AbsoluteValueProblem.h
        Problems/ShiftedQuadratic.h)
