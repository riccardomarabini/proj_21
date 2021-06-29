cmake_minimum_required(VERSION 3.16)
project(SIR)
enable_testing()

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable( main SIR.cpp main.cpp)
target_compile_options(main PRIVATE -Wall -Wextra -fsanitize=address )
target_link_options(main PRIVATE -fsanitize=address)

add_executable(SIR.test SIR.cpp SIR.test.cpp)
target_compile_options(SIR.test PRIVATE -Wall -Wextra -fsanitize=address )
target_link_options(SIR.test PRIVATE -fsanitize=address)
add_test(COMMAND SIR.test)

add_executable( main.graphs SIR.cpp graph.cpp main.graphs.cpp)
target_compile_options(main.graphs PRIVATE -Wall -Wextra -fsanitize=address)
target_link_libraries(main.graphs PRIVATE sfml-graphics)
target_link_options(main.graphs PRIVATE -fsanitize=address)

add_executable( main.world SIR.cpp graph.cpp environment.cpp main.world.cpp)
target_compile_options(main.world PRIVATE -Wall -Wextra -std=c++17 -fsanitize=address)
target_link_libraries(main.world PRIVATE sfml-graphics)
target_link_options(main.world PRIVATE -fsanitize=address)

add_executable(environment.test environment.cpp environment.test.cpp)
target_compile_options(environment.test PRIVATE -Wall -Wextra -fsanitize=address )
target_link_options(environment.test PRIVATE -fsanitize=address)
add_test(COMMAND environment.test)

