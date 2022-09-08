# DockerFile概述
Docker可以通过dockerfile上的指令构建镜像，其本质是一个包含了用户构建镜像所需的所有命令行指令的文本文件。
使用docker build 创建一个连续执行多个命令行指令的自动镜像构建。这也就使得image可以通过复制dockerfile进行便捷镜像复制。  
基本指令：  
```
FROM ubuntu:18.04  # 从父镜像创建基础层，只读不写
COPY . /app        # 从当前目录下添加文件
RUN make /app      # 执行make . 构建指令
CMD python /app/app.py  # 指定容器的运行命令
```
## 概念
1. build context
当cli端发出docker build命令时，当前目录中的文件和所有递归的内容称为build context，会被发送到Docker的守护进程（服务端）中，也可使用-f进行指定dockerfile位置，一般是放在当前目录下。
```
# 下面一系列指令能构建一个最小的临时容器。
# 创建一个build context目录并cd进去，向hello文件中写入hello，并如其创建一个dockerfile用于cat链接它，从当前目录的build context构建镜像。

$ mkdir myproject && cd myproject  # 创建文件夹
$ echo "hello" > hello             # 写入hello到hello
$ echo -e "FROM busybox\nCOPY /hello /\nRUN cat /hello" > Dockerfile   # 写入docker最小构建指令
$ docker build -t helloapp:v1 .    # docker build 构建镜像
Move Dockerfile and hello into separate directories and build a second version of the image (without relying on cache from the last build). Use -f to point to the Dockerfile and specify the directory of the build context:

$ mkdir -p dockerfiles context
$ mv Dockerfile dockerfiles && mv hello context
$ docker build --no-cache -t helloapp:v2 -f dockerfiles/Dockerfile context
```
## 使用
