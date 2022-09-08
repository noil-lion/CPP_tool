# creat by zihao

cmake_minimum_required(VERSION 2.8)

project(BreakPad_demo)

message("\n #### A breakpad demo link library files  #### \n")

set(CMAKE_CXX_STANDARD 17)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)  # 设置库文件输出路径变量，这里设置为代码源目录下的lib文件夹中。
set(BreakPad_INCLUDE_DIR /work/wuzihao/breakpad/breakpad)
# 设置第三方库文件路径(这里全部的第三方库都保存在LIB_FILE变量中)
file(GLOB LIB_FILE ${PROJECT_SOURCE_DIR}/lib/*.a)

# find_package(BreakPad REQUIRED)    # 找库，这里REQUIRED表示一定要找到Opencv这个库，不然不继续编译，也就没法生成exe文件

include_directories(              # 项目的头文件目录，如果用find_package()去找什么库，那可以用这个来指明这个库所要的头文件的路径。
        ${PROJECT_SOURCE_DIR}/include/
        ${BreakPad_INCLUDE_DIR}/src/
)


add_executable(main main.cpp)
# 添加链接库文件，根据调库的原则，一是要有库的头文件，二是要有库生成的共享库或者静态库文件，也就是编译生成.so或者.a文件。
target_link_libraries(main 
        ${LIB_FILE}   
)