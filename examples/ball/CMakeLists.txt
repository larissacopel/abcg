project(asteroids4)
add_executable(${PROJECT_NAME} main.cpp window.cpp bullets.cpp
                               base.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})
