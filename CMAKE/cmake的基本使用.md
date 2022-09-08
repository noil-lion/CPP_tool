***
# cmake基本使用
指明项目相关文件及依赖,对项目进行编译并生成可执行文件.  

```
cmake_minimum_required(VERSION 3.9)  # 最低版本
project(helloworld VERSION 1.0)  # 项目名称及自定义版本号
target_include_directories(hello_cmake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  # 添加头文件路径, PUBLIC关键字指定影响范围,include_directories也起到这个作用
file(GOLB_RECURSE SRC_FILES src/*.cpp)       # 源文件路径
add_executable(hello ${SRC_FILES})           # 将src源文件都变成可搜索,编译生成可执行hello,${SRC_FILES}是路径变量
add_executable(hello_cmake main.cpp)  # 添加生成可执行文件

# 静态库.a和动态库.so链接:静态库在编译时就载入了可执行程序,相当于融合了,会让程序变大,动态库在编译时不融合,在可执行程序执行时才载入内存进行调用,使得生成的可执行程序代码体积更小. 其实相当于导包.

add_library(mylib STATIC lib/blah.cpp)  # STATIC 静态 #添加自定义名为mylib的库文件blah包.
add_executable(hello_cmake main.cpp)  # 添加main.cpp源文件生成可执行文件hello_cmake
target_link_libraries(hello_cmake PUBLIC mylib)  # 链接库文件mylib.


# 导入外部依赖包: 首先从软件仓库下载该库的源代码,再在cmakelist里导包.
find_package(SFML 2 REQUIRED network audio graphics window system)
target_include_directories(hello_cmake PUBLIC ${SFML_INCLUDE_DIR}) 
target_link_libraries(hello_cmake PUBLIC ${SFML_LIBRARIES} ${SFML_DEPENDENCIES}) #链接库文件
```
cmake . 编译源文件   
make 生成可执行  
./executable_hello 运行可执行  

编译和生成两步会生成很多的编译中间文件,这些文件我们用build文件夹来统一保存.  src放置源代码,bin(binary)放最终的可执行文件,include放头文件,lib放链接库文件.  
***

## Cmake第三方库集成
***
1. 外部构建的集成方式  
* 添加第三方被依赖项目的Cmake及库的安装路径
```
add_subdirectory(thirdParty/abc_path  ${DEPS_PATH}/libs)   #添加第三方被依赖项目Cmake及库的安装路径
add_library(abc_static STATIC ${abc_SRCS})  #生成的第三方静态库
```
* 直接添加已经在外部构建好的已经预编译的包,将第三方库和可执行文件直接链接
```
add_executable(xxx_server ${SRC_LIST})
TARGET_LINK_LIBRARIES( abc_static )    #把第三方库连接进去
```

2. [从第三方库的源码开始编译构建](https://www.cnblogs.com/my_life/articles/14479973.html)  
利用 CMake 的 FetchContent 模块来管理依赖。 FetchCotent 是 CMake 3.11 版本开始引入的依赖管理模块.  
分为两种情况,第三方库是否也使用Cmake构建项目.这里主要阐述使用cmake构建的第三方库的集成.

* FetchContent模块核心函数FetchContent_Declare(声明信息)
* FetchContent模块核心函数FetchContent_Populate(下载代码)
