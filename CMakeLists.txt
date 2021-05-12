cmake_minimum_required(VERSION 3.16)
project(SIR)
enable_testing()

set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable( main SIR.cpp main.cpp)
target_compile_options(main PRIVATE -Wall -Wextra  )


add_executable(SIR.test SIR.cpp SIR.test.cpp)
target_compile_options(SIR.test PRIVATE -Wall -Wextra )
add_test(COMMAND SIR.test)

add_executable( main.SFML SIR.cpp graph.cpp main.SFML.cpp)
target_compile_options(main.SFML PRIVATE -Wall -Wextra -fsanitize=address)
target_link_libraries(main.SFML PRIVATE sfml-graphics)
target_link_options(main.SFML PRIVATE -Wall -Wextra -fsanitize=address)

