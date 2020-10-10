# Makefile

## 1、一个规则
目标：依赖条件
    一个tab的缩进

- 目标必须晚于依赖的条件，否则更新目录
- 依赖条件如果不存在，找寻新的规则去产生依赖
- ALL:指定makefile的最终目标

```makefile
#一步
hello:hello.c
    gcc hello.c -o hello
#两步
hello:hello.o
    gcc hello.o -o hello

hello.o:hello.c
    gcc -c hello.c -o hello.o
```

## 2、两个函数

1. `src = $(wildcard *.c)`
   找到当前目录下所有后缀为`.c`的文件赋值给src.
2. `obj = $(patsubst %c,%.o,$(src))`
   把src变量里所有后缀为`.c`的文件替换成`.o`文件

clean:（不存在依赖）


## 3、三个自动变量

- `$@`
  在规则的命令中，表示规则中的目标
- `$<`
  在规则的命令中，表示所有依赖条件
- `$^`
  在规则的命令中，表示第一个依赖条件，如果该变量在模式规则中，它将依赖规则中的条件依次取出，套用模式规则

模式规则：
```makefile
%.o:%c
  gcc -c %< -o $@
```

静态模式规则
```makefile
$(obj):%.o:%.c
```