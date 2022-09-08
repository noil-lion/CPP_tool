# Breakpad 概述
![var](../pic/breakpad.png)
## Breakpad组件
1. 客户端  
客户端是一个库，包含在您的应用程序中。 它可以获取当前线程的状态和当前加载的可执行文件和共享库的ID写转储文件。您可以配置客户端发生了崩溃时写入一个minidump时，或明确要求时。

2. 符号卸载器  
一个程序，读取程序编译时，编译器所产生的调试信息，并生成一个Breakpad格式的调试符号文件。

3. minidump处理器  
读取客户端程序崩溃时生成的minidump文件（错误信息），找到崩溃程序对应版本的调试符号文件（源程序概括），生成一个可读性好的C/C++堆栈跟踪文件。

## 前置概念
1. Core dump  
core是unix系统内核，当程序出现内存越界时，操作系统会中止（kill）当前进程，并将当前的内存状态转储成core文件，它记录了程序挂掉时的详细的状态描述，这样我们可以通过分析core文件找到问题所在。  
core dump又称为核心转储，当程序在运行过程发生异常，操作系统将把程序当前的内存状况存储在一个core文件中，叫做core dump。使用bgd调试器可查看文件详细信息。

2. 调试符号(debug-symbol)格式  
调试符号是编译器在将源文件编译为可执行程序的过程中，为支持调试而摘录的调试信息。这些信息以表格的形式记录在符号表中，是对源程序的概括。调试信息描述目标有变量、类型、函数、符号、源代码行等（函数名或行号，堆栈帧的边界）。  
调试信息时编译过程中逐步收集提炼的，通常情况下是由链接器或专门的工具保存到调试符号文件中，如VS编译器默认将调试符号文件保存到单独的PDB文件，可以使用DbgHelp函数库打开，也可以用DIA SDK。  

## 小型转储文件格式(minidump)
转储文件的格式是由微软开发的类似存储的文件，便于崩溃文件上传。一个minidump文件包含：
* 创建dump的进程加载的可执行文件和共享库列表，包含加载的那些文件的文件名和标识符。

* 程序执行过程中存在的线程列表。对于每个线程转储包括处理器寄存器的状态，线程的堆栈存储器的内容等信息

* 其他系统转储信息：处理器和操作系统版本，转储的原因等

## 核心优势
* minidump轻便易传输
* 兼容跨平台


## 编译使用
1. 创建文件夹
```
mkdir breakpad
cd breakpad
```
2. 下载源码
```
git clone  https://github.com/feifei-123/breakpad
```
breakpad 是跨平台的，支持linux、window和Mac os系统，不同平台上的编译配置也是不同的。
linux 平台编译出来的dump_syms 仅能再linux上运行，来解析linux上运行的so的符号信息
macOS 平台编译出来的dump_syms 仅能再mac OS 上运行，来解析mac 上运行的so的符号信息。
window 平台编译出来的dump_syms，仅能在Window上运行，并解析window上运行的dll的符号信息。  

3. [错误修正]()
* 添加缺失文件  
https://github.com/adelshokhy112/linux-syscall-support
https://blog.csdn.net/weixin_41330194/article/details/124349477
* 修改部分不兼容代码  
![var](../pic/error1.png)
elf_reader.cc文件200-201修改std::strcmp 为 strcmp
* [编译缺失依赖](https://www.cnblogs.com/tsblog/p/16284918.html)  
通过cmakelist配置set(CMAKE_CXX_FLAGS -pthread)解决

4. 编译生成库
```
# Breakpad 提供了一个 Autotools 构建系统，它将构建 Linux 客户端库和处理器库。
cd breakpad
./configure 
make
```
在 ./breakpad/src/client/linux/ 路径下生成libbreakpad_client.a文件（client静态库文件），该库与程序一起编译用于从应用程序中生成minidump文件。  
```
./breakpad/src/client/linux/
libbreakpad_client.a
```
在 ./breakpad/src/tools/linux/dump_syms/ 目录下生成用于提取sym符号文件的可执行程序和.o目标文件还有对应的.cc文件。
```
./breakpad/src/tools/linux/dump_syms/
dump_syms 
dump_syms-dump_syms.o 
dump_syms.cc
```
在 ./breakpad/src/processor/ 目录下生成用于将minidump文件（.dmp）和调试符号文件(.sym)进行处理合成可读的堆栈信息
```
./breakpad/src/processor/
minidump_stackwalk
minidump_stackwalk.cc
minidump_stackwalk_machine_readable_test
minidump_stackwalk.o
minidump_stackwalk_test
```

## Breakpad demo构建

## 项目结构
```
demo
│  ├─ build   # 构建文件夹
│  │  ├─ CMakeCache.txt
│  │  ├─ CMakeFiles
│  │  ├─ Makefile
│  │  ├─ cmake_install.cmake
│  │  ├─ compile_commands.json
│  │  ├─ main      # 编译生成的主程序可执行文件
│  │  └─ main.sym  # 主程序的符号文件
│  ├─ dump_syms    # dump_syms 可执行程序
│  │  ├─ .deps
│  │  │  ├─ .dirstamp
│  │  │  └─ dump_syms-dump_syms.Po
│  │  ├─ .dirstamp
│  │  ├─ dump_syms
│  │  ├─ dump_syms-dump_syms.o
│  │  └─ dump_syms.cc
│  ├─ include     # 头文件目录
│  ├─ lib         # 库文件目录
│  │  └─ libbreakpad_client.a     # breakpad_client静态库
│  ├─ main.cpp                    # 主程序
│  ├─ readme.md
│  ├─ src
│  │  └─ demo.cpp
│  └─ tmp                         # minidump文件存储位置
│     ├─ b551df6b-f6a6-4b22-5d98a480-8fada61e.dmp
│     ├─ bc30faeb-cab2-4cb0-dc7cd584-0235bbc4.dmp
│     └─ minidump.dmp             # minidump实例
```
## 静态库集成
编译好的静态库使用cmake进行导入
```
# creat by zihao

cmake_minimum_required(VERSION 2.8)

project(BreakPad_demo)

message("\n #### A breakpad demo link library files  #### \n")
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)  # 设置库文件输出路径变量，这里设置为代码源目录下的lib文件夹中。
set(BreakPad_INCLUDE_DIR /work/wuzihao/triton/BreakPad/breakpad)
# 设置第三方库文件路径(这里全部的第三方库都保存在LIB_FILE变量中)
file(GLOB LIB_FILE ${PROJECT_SOURCE_DIR}/lib/*.a)

# find_package(BreakPad REQUIRED)    # 找库，这里REQUIRED表示一定要找到Opencv这个库，不然不继续编译，也就没法生成exe文件

include_directories(              # 项目的头文件目录，如果用find_package()去找什么库，那可以用这个来指明这个库所要的头文件的路径。
        ${PROJECT_SOURCE_DIR}/include/
        ${BreakPad_INCLUDE_DIR}/src/
        /usr/include/
)

set(CMAKE_CXX_FLAGS -pthread)  # lpthread_once错误
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
add_executable(main main.cpp)
# 添加链接库文件，根据调库的原则，一是要有库的头文件，二是要有库生成的共享库或者静态库文件，也就是编译生成.so或者.a文件。
target_link_libraries(main 
        ${LIB_FILE}   
)
```
## 执行
1. 生成minidump
```
$ cd build
$ ./main
```
2. 为应用程序生成符号文件并创建规定存放目录  
```
$ cd ../../
$ dump_syms/dump_syms ./demo/build/main > main.sym
> MODULE Linux x86_64 5D3648439FC02862D36B4082842CBA300 main
$ mkdir -p ./symbols/main/5D3648439FC02862D36B4082842CBA300
$ mv main.sym ./symbols/main/5D3648439FC02862D36B4082842CBA300/
```
3. 处理minidump文件生成堆栈跟踪
```
$ breakpad/src/processor/minidump_stackwalk ./tmp/minidump.dmp ./symbols
``` 
4. 缺失libc.so.6 的sym文件也如上进行创建，首先得找到libc.so.6，重复2-3步骤
## 结果查看
```
2022-09-07 17:02:29: minidump_processor.cc:381: INFO: Processed tmp/minidump.dmp
Operating system: Linux
                  0.0.0 Linux 5.15.0-43-generic #46~20.04.1-Ubuntu SMP Thu Jul 14 15:20:17 UTC 2022 x86_64
CPU: amd64
     family 6 model 106 stepping 6
     1 CPU

GPU: UNKNOWN

Crash reason:  SIGSEGV /SEGV_MAPERR
Crash address: 0x0
Process uptime: not available

Thread 0 (crashed)
 0  main!crash() + 0x14
    rax = 0x0000000000000000   rdx = 0x0000000000000000
    rcx = 0x0000562ef9b92090   rbx = 0x0000562ef8a1ca60
    rsi = 0x0000000000000000   rdi = 0x0000562ef8a256c0
    rbp = 0x00007ffce1c59240   rsp = 0x00007ffce1c59240
     r8 = 0x0000000000000000    r9 = 0x0000562ef9b92088
    r10 = 0x0000000000000008   r11 = 0x00007fd5c6d71be0
    r12 = 0x0000562ef8a09f40   r13 = 0x00007ffce1c594f0
    r14 = 0x0000000000000000   r15 = 0x0000000000000000
    rip = 0x0000562ef8a0a07c
    Found by: given as instruction pointer in context
 1  main!main + 0xcc
    rbx = 0x0000562ef8a1ca60   rbp = 0x00007ffce1c59400
    rsp = 0x00007ffce1c59250   r12 = 0x0000562ef8a09f40
    r13 = 0x00007ffce1c594f0   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x0000562ef8a0a151
    Found by: call frame info
 2  libc.so.6!__libc_start_main + 0xf3
    rbx = 0x0000562ef8a1ca60   rbp = 0x0000000000000000
    rsp = 0x00007ffce1c59410   r12 = 0x0000562ef8a09f40
    r13 = 0x00007ffce1c594f0   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x00007fd5c6ba9083
    Found by: call frame info
 3  main!_start + 0x2e
    rbx = 0x0000000000000000   rbp = 0x0000000000000000
    rsp = 0x00007ffce1c594e0   r12 = 0x0000562ef8a09f40
    r13 = 0x00007ffce1c594f0   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x0000562ef8a09f6e
    Found by: call frame info
 4  0x7ffce1c594e8
    rbx = 0x0000000000000000   rbp = 0x0000000000000000
    rsp = 0x00007ffce1c594e8   r12 = 0x0000562ef8a09f40
    r13 = 0x00007ffce1c594f0   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x00007ffce1c594e8
    Found by: call frame info

Loaded modules:
0x562ef8a02000 - 0x562ef8a1cfff  main  ???  (main)
0x7fd5c6a36000 - 0x7fd5c6ae9fff  libm.so.6  ???
0x7fd5c6b85000 - 0x7fd5c6d1efff  libc.so.6  ???
0x7fd5c6d77000 - 0x7fd5c6d8dfff  libpthread.so.0  ???
0x7fd5c6d9a000 - 0x7fd5c6daefff  libgcc_s.so.1  ???
0x7fd5c6db5000 - 0x7fd5c6f3bfff  libstdc++.so.6  ???
0x7fd5c6fa8000 - 0x7fd5c6fcbfff  ld-linux-x86-64.so.2  ???
0x7ffce1c8a000 - 0x7ffce1c8bfff  linux-gate.so  ???
2022-09-07 17:02:29: minidump.cc:5288: INFO: Minidump closing minidump
```
