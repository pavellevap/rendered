cmake_minimum_required(VERSION 3.4)
project(Renderer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp  -lSDL2 -lSDL2main -lSDL_ttf -pthread -std=c++11")

set(SOURCE_FILES main.cpp Scene.cpp Scene.h Primitives/Primitive.h Primitives/Triangle.cpp Primitives/Triangle.h
        Primitives/Sphere.cpp Primitives/Sphere.h Renderer.cpp Renderer.h Linalg/Point.cpp Linalg/Point.h
        Linalg/Matrix.cpp Linalg/Matrix.h RGB/RGBImage.cpp RGB/RGBImage.h Linalg/Vector.cpp Linalg/Vector.h
        Linalg/Ray.cpp Linalg/Ray.h RGB/RGBColor.cpp RGB/RGBColor.h LightSources/PointLight.cpp LightSources/PointLight.h
         Object.cpp Object.h Primitives/Quadrangle.cpp Primitives/Quadrangle.h
        LightSources/AmbientLight.cpp LightSources/AmbientLight.h KDTree.cpp KDTree.h Thing.h Box.cpp Box.h
        Concurrency/ThreadSafeQueue.h Concurrency/ThreadPool.h Material.cpp Material.h Camera.cpp Camera.h
        Intersectable.h Boundable.h Primitives/Parallelepiped.cpp Primitives/Parallelepiped.h
        ObjectArithmetic/Union.cpp ObjectArithmetic/Union.h ObjectArithmetic/Complement.cpp ObjectArithmetic/Complement.h
        ObjectArithmetic/Difference.cpp ObjectArithmetic/Difference.h Closed.h ClosedThing.cpp ClosedThing.h
        ObjectArithmetic/Intersection.cpp ObjectArithmetic/Intersection.h)



add_executable(Renderer ${SOURCE_FILES})


find_library(SDL_LIB SDL2)
find_library(SDLMAIN_LIB SDL2main)
find_library(SDLTTF_LIB SDL_ttf)

target_link_libraries(Renderer ${SDL_LIB} ${SDLMAIN_LIB} ${SDLTTF_LIB})