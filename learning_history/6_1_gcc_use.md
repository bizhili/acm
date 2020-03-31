### gcc
1.gcc main.c //编译main.c,生成a.out文件，./a.out运行
2.gcc -S -o main.s main.c//反汇编到main.s中
### gcc编译过程
1.预处理
gcc -E
替换了宏的C源程序(没有了#define,#include…), 删除了注释
2.编译
gcc -S
汇编源程序
3.汇编
gcc -c
目标文件，二进制文件， 允许有不在此文件中的外部变量、函数
4.链接
gcc
可执行程序，一般由多个目标文件或库链接而成， 二进制文件，所有变量、函数都必须找得到

