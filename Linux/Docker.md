# Docker 概念

image

container

registry

# 使用镜像

pull

list

rm

commit

# Dockerfile

`CMD`和`ENTRYPOINT`等支持 shell 格式和 json 数组格式。

`FROM <image> [as <name>]`从上层镜像构建自己的镜像。

如果源路径为文件夹，`COPY` 的时候不是直接复制该文件夹，而是将文件夹中的内容复制到目标路径。

 不建议使用 `ADD`

Docker 不是虚拟机，容器中的应用都应该以前台执行，而不是像虚拟机、物理机里面那样，用 `systemd` 去启动后台服务，容器内没有后台服务的概念。

`ENTRYPOINT`使得用户可以将 docker 镜像当成一般命令使用，传递给docker 的参数会传递给`ENTRYPOINT`，然后由`ENTRYPOINT`执行。

`ARV`设置**构建阶段**环境变量，构建后消失，可以使用`docker history`查看，在`FROM`之前设置的`ARV`只能用于该`FROM`中，分段构建时要分别指定`ARV`。使用`$name`访问

`VALUME <path>[, <path...]`设置镜像的匿名卷，避免用户忘记挂载导致数据丢失，用户可以手动设置卷在物理机中的目录。

`EXPOSE <端口1> [<端口2>...]`**声明**容器向外暴露的端口，这个指令仅仅是声明，并不实际暴露端口。实际暴露端口需要用`-P`命令自动映射，或`-p`手动映射。

`WORKDIR`用于切换当前目录，目录不存在就自动创建。如:

```dockerfile
WORKDIR /app

RUN echo "hello" > world.txt
```

`USER <user>[:<group>]`切换当前用户。如：

```dockerfile
RUN groupadd -r redis && useradd -r -g redis redis
USER redis
RUN [ "redis-server" ]
```

在 tty 缺失的环境中，`su`和`sudo`可能出问题，如果要在命令中切换用户，可以使用[gosu](https://github.com/tianon/gosu)：

```dockerfile
# 建立 redis 用户，并使用 gosu 换另一个用户执行命令
RUN groupadd -r redis && useradd -r -g redis redis
# 下载 gosu
RUN wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/1.12/gosu-amd64" \
    && chmod +x /usr/local/bin/gosu \
    && gosu nobody true
# 设置 CMD，并以另外的用户执行
CMD [ "exec", "gosu", "redis", "redis-server" ]
```

`HEALTHCHECK [<--INTERNAL>=<time> ] [<--timemout>=<time>] [<--retries>=<count>] CMD <command>`检查 docker 健康状态，`<command>`返回值：

- `0`：成功

- `1`：失败

- `2`：保留

`<comand>`中的所有 stdout/stderr 输出都会被记录下来，可以通过`docker inspect <container>`查看。

容器的健康状态在`docker container ls`的`STATUS`中。

`ONBUILD <docker-instruaction>`后面接一般的 Dockerfile 指令，如`COPY`等，唯一的区别在于，这个指令会在用户构建下一级镜像时执行。

镜像 mynode

```dockerfile
FROM node:slim
RUN mkdir /app
WORKDIR /app
ONBUILD COPY ./package.json /app
ONBUILD RUN [ "npm", "install" ]
ONBUILD COPY . /app/
CMD [ "npm", "start" ]
```

用户基于 mynode 构建自己的镜像

```dockerfile
FROM my-node
```

`LABEL <key>=<value> [<key>=<value>...]`可以为镜像添加元数据参考[Annotations](https://github.com/opencontainers/image-spec/blob/main/annotations.md)。

`SHELL \["<executable>", "<parameters>"\]`设置 Dockerfile 中执行命令的 shell，默认是`["/bin/sh", "-c"]`。

多阶段构建：

多次`FROM`（可以使用`as`设置别名），`COPY`的`<--from>=<image>`从某层镜像拷贝，`--from=0`从上层拷贝。



镜像导出为 tarball：`docker save <image> -o <file>`，还可以用 gzip 压缩，`$ docker save <image> | gzip > <image>.tar.gz`。导入镜像：$ `docker load -i <file>`。

# 操作容器



# 设置仓库



# 数据管理

docker volume ls/rm/inspect/create

清除无用的 valume，docker volume rm prune

挂载卷/主机目录默认是可读写的，如果要只读挂载：

- -v: `-v src:target:ro`
- --mount: `--mount type=bind,source=<src>,target=<target>,readonly`

# 网络管理

# Docker-compose

# docker 与云

# 调试 docker

# docker 最佳实践