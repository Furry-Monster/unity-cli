cmake_minimum_required(VERSION 3.16)
project(unity-cli VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

# 设置头文件目录
include_directories(include)

# 源文件
set(SOURCES
  src/main.cpp
  src/command_manager.cpp
  src/clean_command.cpp
  src/structure_command.cpp
)

# 头文件
set(HEADERS
  include/command_interface.h
  include/command_manager.h
  include/clean_command.h
  include/structure_command.h
)

# 创建可执行文件
add_executable(unity-cli ${SOURCES} ${HEADERS})

# 链接文件系统库 (GCC 8及以下版本需要)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(unity-cli stdc++fs)
endif()

# 安装配置
install(TARGETS unity-cli
  RUNTIME DESTINATION bin
)

# 设置输出目录
set_target_properties(unity-cli PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
