# Docker compose概述
Compose是一个用于定义和允许多容器docker应用程序的工具，使用YAML文件来配置应用程序的服务。  

## Compose 基本概念
* 管理应用程序整个生命周期  
* 单个主机上的多个隔离环境  
* 创建容器时保留卷数据 [持久存储数据]  
* 仅重新创建已更改的容器  
* 变量和在环境之间移动组合  

## 基本使用流程
1. 使用Dockerfile定义app的环境，以便于将其在任何地方复制  
2. 使用docker-compose.yml组织这些app来构建services。  
3. 执行docker compose up 指令启动并运行整个app。

```
# 一个典型的docker-compose文件如下
version: "3.9"  # optional since v1.27.0
services:
  web:
    build: .  # 构建指令
    ports:
      - "8000:5000"  # 端口映射
    volumes:
      - .:/code      # 数据挂载
      - logvolume01:/var/log
    depends_on:    # 依赖项
      - redis
  redis:
    image: redis
volumes:
  logvolume01: {}
```

## 快速入门
使用docker-compose快速构建多容器部署环境
1. 创建目录并完成编码
```
 mkdir composetest
 cd composetest

# app.py
from flask import Flask
from redis import StrictRedis
import os
import socket

app = Flask(__name__)
redis = StrictRedis(host=os.environ.get('REDIS_HOST', '127.0.0.1'),
                    port=6379, password=os.environ.get('REDIS_PASS')) # 从环境变量读密码


@app.route('/')
def hello():
    redis.incr('hits')
    return f"Hello Container World! I have been seen {redis.get('hits').decode('utf-8')} times and my hostname is {socket.gethostname()}.\n"

```
创建一个flask项目，以redis服务进行数据存储  
requirements.txt文件
```
flask
redis
```
2. 创建dockerfile
以下dockerfile可以构建一个flask程序的运行环境
```
# syntax=docker/dockerfile:1
FROM python:3.7-alpine  # 从 Python 3.7 映像开始构建映像。
WORKDIR /code  # 将工作目录设置为/code
ENV FLASK_APP=app.py  
ENV FLASK_RUN_HOST=0.0.0.0  # 设置命令使用的环境变量flask
RUN apk add --no-cache gcc musl-dev linux-headers  # 安装 gcc 和其他依赖项
COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt  # 复制requirements.txt并安装 Python 依赖项。
EXPOSE 5000  # 描述容器正在侦听端口 5000
COPY . .    # 将项目中的当前目录复制.到镜像中的workdir .
CMD ["flask", "run"]  # 将容器的默认命令设置为flask run
```

3. 在 Compose 文件中定义服务
创建一个名为的文件docker-compose.yml，以下文件定义了俩服务，一个是web服务，还有一个是redis存储服务。
```
version: "3.9"
services:
  web:
    build: .  # 使用上面的dockerfile构建容器指令
    ports:
      - "8000:5000"   # 将容器的flask监听端口5000与主机的端口8000绑定
    volumes:
      - .:/code    # 挂载当前项目目录到容器的code目录下，这允许开发人员即时修改而不需要重启服务。
    nvironment:
      FLASK_ENV: development  # 告诉flask run在开发模式下运行并在更改时重新加载代码
  redis:
    image: "redis:alpine"  # 基于公共镜像提供redis服务
```

4. Compose构建应用程序
在项目目录中，运行docker compose up,Compose 拉取Redis镜像，启动定义的俩服务，这时，项目的源代码会被copy到构建的镜像中，启动后提供web服务和redis服务。doker compose down可以停止应用程序。
```
$ docker compose up
```

5. 分离模式
如果想在后台运行服务，可以使用-d标志（用于“分离”模式）传递给docker compose up  
并用于docker compose ps查看当前正在运行的服务
```
 docker compose up -d
```