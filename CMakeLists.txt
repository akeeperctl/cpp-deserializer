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
  "src/types/Integer.h" "src/types/Float.h" "src/types/String.h" "src/types/Vector.h" "src/types/Any.h" "src/types/Common.h"  "src/types/SerializableValue.h" "src/main.cpp")
