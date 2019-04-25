# dlcc - 简单 C 语言编译器

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

dlcc 可接受参数：

- -i，解释器模式，立刻执行
- -s，汇编器模式，产生汇编代码
- -o，指明生成的汇编代码输出文件

```shell
# 例子
./dlcc ../test/a.c -i
./dlcc ../test/a.c -s -o ../test/a.s
```




## 说明文档

详见 doc

