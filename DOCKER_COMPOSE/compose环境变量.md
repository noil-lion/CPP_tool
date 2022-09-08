# docker compose环境变量
在docker-compose.yml文件中使用环境变量来控制不同的条件和使用场景，docker compose支持多级设置环境变量，其实粗糙地理解就是通过环境变量的设置，可以灵活地控制文件中的各中版本号、或其他因使用场景不同而不同的参数值。
DC文件中引用的环境变量是有优先级的：
```
Compose file                 # compose文件里的变量
Shell environment variables  # shell命令行变量
Environment file             # 环境变量
Dockerfile                   # dockerfile中的变量
Variable is not defined      # 未定义变量
```
__如果有多个环境变量，可以通过将它们添加到名为的默认环境变量文件.env或使用--env-file命令行选项提供环境变量文件的路径来替换它们。__

## 例子
1. ${POSTGRES_VERSION}为环境变量，控制postgresql的版本。在up的时候，会先在本文件里找这个变量，然后时shell命令行的变量的值，然后时环境变量文件里定义的变量，最后会去dockerfile中去找。
```
db:
  image: "postgres:${POSTGRES_VERSION}"
```