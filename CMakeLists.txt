cmake_minimum_required(VERSION 3.16)
project(SF2565_PROJECT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SF2565_PROJECT
        main.cpp
        bench_driver.cpp
        AHA_Optimizer.cpp
        AHA_Optimizer.h
        VisitTable.cpp
        VisitTable.h
        Hummingbird.h
        FlightEngine.cpp
        FlightEngine.h
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
        Problems/Ackley.h
        Problems/Zakharov.h
        Problems/Michalewicz10.h
        Problems/Shekel10.h
        Problems/SixHumpCamelBack.h
        Problems/Beale.h
        Problems/Branin.h
        Problems/Powell.h
        Problems/Easom.h
)