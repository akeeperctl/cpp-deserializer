cmake_minimum_required (VERSION 3.8)

project ("TestTaskIniti")

set(CMAKE_CXX_STANDARD 17)

add_custom_command(
    OUTPUT raw
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/raw.bin ${CMAKE_CURRENT_BINARY_DIR}/raw.bin
)

add_executable (Main
    "src/main.cpp"
    raw
  "src/types/integer.h" "src/types/float.h" "src/types/string.h" "src/types/vector.h" "src/types/any.h" "src/types/common.h" )
