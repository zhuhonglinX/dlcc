# dlcc - 简单 C 语言编译器

## 简介

![工作流程图](./images/image3.png)

输出：

- 生成 linux 32位汇编代码
- 立刻执行



## 使用

使用 cmake 构建项目，创建 build 目录

```shell
cd build
cmake ..
make
```

会在 build 下生成 dlcc 可执行文件。

![image2](./images/image2.png)



dlcc 可接受参数：

- -i，解释器模式，立刻执行
- -s，汇编器模式，产生汇编代码
- -o，指明生成的汇编代码输出文件

```shell
# 例子
dlcc -i source.c
dlcc -s source.c -o source.s
```



## GUI Editer

python3

Tkinter

简易的图形化代码编辑器，集成 dlcc 编译器。使用之前需要先把 dlcc 加入环境变量

![image1](./images/image1.png)



