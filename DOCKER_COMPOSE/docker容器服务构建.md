# 容器构建-GPU资源限制
目标：创建一个基于triton镜像的指定GPU设备的容器服务环境并启动

## 服务容器的GPU资源限制
Docker Compose v1.28.0+ 通过定义device结构来为服务预留GPU资源，且提供了对GPU资源更精细的控制。通过以下属性进行：
* capabilities -  值指定为字符串列表（例如。capabilities: [gpu]）。必须在 Compose 文件中设置此字段。
* count -  指定为 int 的值或all表示应保留的 GPU 设备数量的值。
* device_ids - 指定为表示来自主机的 GPU 设备 ID 的字符串列表的值。
* driver - 指定为字符串的值（例如driver: 'nvidia'）。
options - - 表示驱动程序特定选项的键值对。
count 和 device_ids不能同时设置，不设置的话默认使用主机上的所有可以GPU

## 例
```
# 用于运行可访问 1 个 GPU 设备的服务的 Compose 文件示例：
services:
  test:
    image: nvidia/cuda:10.2-base
    command: nvidia-smi
    deploy:
      resources:
        reservations:
          devices:
            - driver: nvidia
              count: 1
              capabilities: [gpu]
```
## [Docker Compose安装使用](https://docs.docker.com/engine/install/ubuntu/)
## [Docker Compose 构建部署示例](https://github.com/docker/awesome-compose/tree/master/django)
## 流程
1. 创建项目文件夹
项目文件结构
```
.
├── compose.yaml
├── app
    ├── Dockerfile
    ├── requirements.txt
    └── manage.py
```

2. 写dockerfile
```
# syntax=docker/dockerfile:1.4

FROM --platform=$BUILDPLATFORM python:3.7-alpine AS builder   # 基镜像
EXPOSE 8000              # 监听端口
WORKDIR /app             # work目录设置
COPY requirements.txt /app   # copy要安装的依赖项目录
RUN pip3 install -r requirements.txt --no-cache-dir  # 安装依赖项
COPY . /app   # 载入项目到镜像
ENTRYPOINT ["python3"]    # 
CMD ["manage.py", "runserver", "0.0.0.0:8000"]  # 容器运行时执行指令

FROM builder as dev-envs
RUN <<EOF
apk update
apk add git
EOF

RUN <<EOF
addgroup -S docker
adduser -S --shell /bin/bash --ingroup docker vscode
EOF
# install Docker tools (cli, buildx, compose)
COPY --from=gloursdocker/docker / /
CMD ["manage.py", "runserver", "0.0.0.0:8000"]
```
3. 写docker compose文件
```
version: '3'  # docker compose版本声明
services:     # 服务配置定义
  test:
    image: nvcr.io/nvidia/tritonserver:22.04-py3   # 构建的镜像
    runtime: nvidia                                # 运行时
    restart: always                                # 自启动
    command: sh -c "tritonserver --model-repository=/work/wuzihao/triton/model_repository --strict-model-config=false"     # 启动时执行的命令
    cap_add:
      - SYS_PTRACE                # 权限组设置
    environment:                  #　指定环境变量
      - LANG=C.UTF-8
    ports:                        # 端口映射
      - "40182:22" # SSH
      - "40183:8000"
      - "40184:8001"
      - "40185:8002"
    deploy:                       # 服务部署时GPU资源限制
      resources:
        reservations:
          devices:
            - driver: nvidia
              device_ids: ['4']
              capabilities: [gpu]
    container_name: zihao_research
    volumes:                    # 文件卷挂载
      - /etc/localtime:/etc/localtime:ro
      - /etc/timezone:/etc/timezone:ro
      - /home/user/wuzihao/work:/work
    shm_size: 32G
    network_mode:
      bridge
    stdin_open:
      true
```
4. 测试验证
```
docker compose up -d
dcoker compose ps
docker inspect

curl -v localhost:8000/v2/health/ready

```
